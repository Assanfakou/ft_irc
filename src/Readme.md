## 1. What is a Container?

In C++, a container is any data structure that holds a collection of objects. It is a broad, generic term. Examples of containers include:
- ```std::vector``` (a dynamic array)
- ```std::list``` (a linked list)
- ```std::map``` (a key-value dictionary)

## 2. What is a Vector?

A vector ```(std::vector)``` is a specific container that stores its elements in one continuous block of computer memory, right next to each other. Because it is a container, its whole job is to hold things—in your case, it holds a list of ```pollfd``` structures.


## 3. What is a Map?

A ```map``` is a **container**, like ```vector``` — but instead of storing items by
position (index 0, 1, 2...), it stores **KEY** -> **VALUE** pairs and looks
things up by key directly, without looping.

In ```ft_irc```, our **"key"** is the client's ```fd``` (an ```int```), and our **"value"** is
their ```Client object``` (nickname, buffer, registration state, etc).
```cpp
  std::map<int, Client> _clients;
```
This matters because ```poll()``` only ever gives us an ```fd``` — a bare integer.
A map lets us jump straight from that ```fd``` to **"everything we know about
this specific client"** in one lookup, instead of searching a vector.

## Core operations we use
```cpp
  _clients[fd] = Client(fd);          // insert a new client (on accept)
  _clients[fd].appendToBuffer(data);  // access/modify an existing client (on recv)
  _clients.erase(fd);                 // remove a client (on disconnect)

  std::map<int, Client>::iterator it = _clients.find(fd);
  if (it != _clients.end())           // safe existence check
  {
      it->first;   // the fd (key)
      it->second;  // the Client object (value)
  }
```
## The [] trap with std::map — explained in full

### First, understand what [] actually does

The `[]` operator on a map does two different things depending on
whether the key exists or not:

- Key EXISTS     → returns a reference to the existing value
- Key DOES NOT EXIST → **silently creates a new entry**, then returns a reference to it

This "silent creation" is called **default construction** — the map
calls the class's no-argument constructor `Client()` to build something
to put there.

### Visualizing the trap

Imagine our map starts empty:

```
_clients = {}
```

Now somewhere in our code we accidentally write:

```cpp
_clients[99].appendToBuffer("hello");
```

There is no client with fd 99. Maybe they already disconnected.
Maybe it's just a bug. Either way, the map doesn't error.
Instead it silently does this:

```
_clients = { 99 -> Client() }   // ghost entry, created from nothing
```

And then calls appendToBuffer on that ghost Client.
No crash. No warning. Just a fake client living in our map,
corrupting our state, and hiding the real bug underneath.

### Why this forces us to have a default constructor

When `[]` decides to create a new entry, it has to construct
a `Client` object with no arguments:

```cpp
Client();   // the map calls this internally
```

If our `Client` class only has:

```cpp
Client(int fd);   // requires an fd argument
```

...and no `Client()`, the compiler refuses to compile any code
that uses `[]` on our map, because it can't fulfill its "create
something if missing" contract.

So we have two choices:

**Option A** — add a default constructor just to keep the compiler happy:

```cpp
Client() : _fd(-1), _registered(false) {}
```

fd = -1 signals "this is an invalid/ghost client."
We don't use this constructor intentionally — it only exists
to satisfy the map's requirement. Every real Client is created
with an actual fd via `Client(int fd)`.

**Option B** — never use `[]` for creation, use `.insert()` instead:

```cpp
_clients.insert(std::make_pair(clientFd, Client(clientFd)));
```

This never default-constructs anything. The Client is built
with a real fd from the start, and if the key already exists,
insert() does nothing (no silent overwrite either).

### The safe rule for our codebase

| situation | what to use | why |
|---|---|---|
| new client just accepted | `insert(make_pair(fd, Client(fd)))` | explicit, no silent creation |
| read/modify existing client | `_clients[fd]` or `.find(fd)` | fd is guaranteed to exist |
| check if client exists | `.find(fd) != .end()` | never accidentally creates |
| client disconnects | `.erase(fd)` | clean removal |

### Quick visual of find() vs []

```cpp
// SAFE — find() never creates anything
std::map<int, Client>::iterator it = _clients.find(fd);
if (it != _clients.end())
{
    it->second.appendToBuffer(data);  // we KNOW it exists
}
else
{
    // fd not found, handle the error
}

// DANGEROUS if fd might not exist
_clients[fd].appendToBuffer(data);  // creates ghost if fd is missing
```

### One-line mental model

`[]` means "give it to me, and if it doesn't exist, make one."
`.find()` means "look for it, and tell me if you found it or not."

Use `[]` only when you KNOW the key already exists, or you
explicitly WANT to create it. Use `.find()` for everything else.

## Why map instead of vector for clients

With ```vector<pollfd>``` we only ever have raw ```fds``` — no place to store
per-client state like nickname or a partial-message buffer. The Client
class will hold that state, and ```map<int, Client>``` lets us go straight
from **"an fd that ```poll()``` just told us about"** to **"that client's full
object"** without looping through a vector to find it.

## ---------------------------------------------------------------------
A vector looks like:
```cpp
index      value

0          fd=3
1          fd=4
2          fd=5
```
You access by position:
```cpp
vec[1]
```

A map looks like:
```cpp
key        value

4          Client(4)
5          Client(5)
6          Client(6)
```
You access by key:
```cpp
_clients[4]

instead of:

_clients[0]
```
## ---------------------------------------------------------------------

## pollfd struct :

```cpp
struct pollfd {
    int   fd;       // which fd you're watching
    short events;   // INPUT: what you're asking to be notified about
    short revents;  // OUTPUT: what actually happened
};
```
It's literally a request/response pair sitting inside the same struct. You fill in ```fd``` and ```events``` before calling ```poll()```. The kernel fills in ```revents``` after ```poll()``` returns.

Step by step, what actually happens

**1. You build the array, before calling poll():**
```cpp
struct pollfd pfd;
pfd.fd = _serverSocket;
pfd.events = POLLIN;   // "tell me if this becomes readable"
pfd.revents = 0;       // doesn't matter yet, kernel will overwrite it
```
You do this for every fd you care about — your listening socket, and every connected client. That's your _pollfds ```vector```.

**2. You call poll():**
```cpp
int ret = poll(_pollfds.data(), _pollfds.size(), -1);
```
At this point your process blocks — it stops running entirely. The kernel takes your whole array, looks at every fd in it, and basically asks each one: "are you readable right now, or do you have anything pending?" If nothing is ready anywhere, your process just sits asleep, using zero CPU, until something changes.

**3. Something happens** — a client sends a message, or a new connection request arrives at your listening socket. The kernel notices this internally (it's already tracking socket buffers, connection queues, etc. — that's just normal kernel bookkeeping it always does).

**The kernel wakes your process back up** and, before returning from ```poll()```, it goes through your array and writes into ```revents``` for every fd that had something happen:
```cpp
_pollfds[i].revents = POLLIN;  // the kernel writes this, not you
```

**5. poll() returns**, and now you inspect ```revents``` — never ```events``` — to see what actually fired:
```cpp
if (_pollfds[i].revents & POLLIN)
{
    // something happened on this fd
}
```

**Why two separate fields instead of one?**

Because a single fd can have multiple events happening at once (readable AND an error, for example), and you might be asking to be notified about multiple things too (```POLLIN``` | ```POLLOUT```). ```revents``` can contain flags you didn't even ask for — like ```POLLHUP``` or ```POLLERR```, which the kernel reports unconditionally regardless of what you put in ```events```. So they can't share one field; ```events``` is your subscription, ```revents``` is the kernel's report card, and they don't always match 1:1.

**One-line mental model**

```events``` = the question you're asking the kernel.

```revents``` = the kernel's answer, written back into the same slot.

You set one before sleeping, the kernel sets the other before waking you up.


## poll() function;

It allows a program to monitor multiple file descriptors at the same time to see if any of them are ready for I/O operations (like reading or writing) without blocking.

```cpp
// poll() expects: (pointer to array, number of items, timeout)
poll(_pollfds.data(), _pollfds.size(), -1);
```

### where did data() and size() come from ?

- **```.size()```** is a standard function included in almost all **C++ containers** (vectors, lists, maps, sets). It simply tells you how many items are currently stored inside. (Number of pollfd structures in the array).
- **```.data()```** is unique to containers that store their elements in a single, uninterrupted row of memory (like ````std::vector``` and ```std::array```). It does not exist in containers like ```std::list``` or ```std::map```.

The ```poll()``` function is old and does not understand modern C++ vectors. It requires two raw ingredients to work:

1. **A pointer to the first item** in a raw array.
2. **The total count** of items in that array.

By writing ```_pollfds.data()``` and ```_pollfds.size()```, they are perfectly bridging the gap between C++ and C.

## why this _pollfds[i].fd == _serverSocket ?

**The core idea:** ```poll()``` watches a list of file descriptors and tells you which ones have activity. But it doesn't tell you what kind of fd it was — you have to figure that out yourself by comparing it to fds you already know about.

In your ```_pollfds``` vector, you have one special fd: ```_serverSocket```. That's the listening socket, the one you created with ```socket()``` and called ```bind()```/```listen()``` on. It was never connected to any specific client — its only job is to sit there waiting for new people to try to connect.

Every other fd in ```_pollfds``` is a client socket, created later by ```accept()``` when someone actually joined.

So when ```poll()``` returns and you loop through ```_pollfds```, checking ```revents``` & ```POLLIN``` just tells you "this fd has something ready to read." But "something ready to read" means a completely different thing depending on which fd it is:

- If the fd that's ready is ```_serverSocket```, it means a new client is trying to connect — there's a pending connection sitting in the kernel's queue waiting for you to call ```accept()``` on it. You can't ```recv()``` from ```_serverSocket``` because no actual client data ever flows through it; it's not a conversation with anyone, it's just a doorbell.

- If the fd that's ready is one of the other fds (a client fd like 4 or 5), it means that specific client already connected earlier and has now sent you actual data — a message, an IRC command, whatever. Here you call ```recv()``` to read what they sent.

That's the entire reason for the ```if (_pollfds[i].fd == _serverSocket)``` check. It's the only way to distinguish **"someone new wants in"** from **"an existing client said something."**
Same event (```POLLIN```), two completely different meanings, depending purely on which fd triggered it.

# -----------------------------------------

# poll() Flow Example

## Server is waiting

The server is currently blocked inside:

```cpp
poll(...)
```

waiting for activity on one of the monitored file descriptors.

---

## A new client tries to connect

The client runs:

```bash
nc localhost 6673
```

The connection request reaches the kernel.

Since the listening socket has a pending connection waiting to be accepted, the kernel internally marks:

```cpp
_serverSocket.revents = POLLIN;
```

Then `poll()` wakes up and returns.

---

## The server checks what happened

The event loop iterates through all monitored file descriptors:

```cpp
if (_pollfds[i].revents & POLLIN)
```

This condition is true because the kernel set `revents` to `POLLIN`.

Next, the server checks:

```cpp
if (_pollfds[i].fd == _serverSocket)
```

This is also true.

That tells us that the file descriptor that became readable is the **listening socket**.

For a listening socket, `POLLIN` does **not** mean that a client sent a message.

It means:

> A new connection is waiting to be accepted.

Therefore the correct action is:

```cpp
acceptClient();
```

---

## Later: an existing client sends data

Suppose a connected client sends:

```text
PRIVMSG hello
```

The kernel receives those bytes and stores them in the client's socket buffer.

The kernel then marks:

```cpp
clientFd.revents = POLLIN;
```

because there is now data waiting to be read.

`poll()` wakes up again and returns.

The event loop finds:

```cpp
if (_pollfds[i].revents & POLLIN)
```

which is true.

This time:

```cpp
_pollfds[i].fd == clientFd
```

and **not**:

```cpp
_pollfds[i].fd == _serverSocket
```

This means the event came from a connected client socket.

For a client socket, `POLLIN` means:

> There is data waiting to be read.

Therefore the correct action is:

```cpp
recv(...);
```

to read the client's message from the kernel buffer.

# -------------------------------------------------

## accept() prototype :

```cpp
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

**What are addr struct for?**

When a client connects, Linux can tell us:
```cpp
Client IP    // 127.0.0.1
Client Port  // port 54123
```
To store that information we need:
```cpp
sockaddr_in clientAddr;  // *addr
socklen_t clientLen = sizeof(clientAddr);  // addrlen
```
As for **addrlen** it means **Size of that structure**.


## Why non-blocking sockets, if poll() already tells us what's ready?

poll() guarantees a fd has *some* data ready at the moment it wakes you up.
It does NOT guarantee all the data you eventually want is available yet.

If a socket is left in default (blocking) mode, and your recv() logic
ever loops waiting for more bytes (e.g. waiting for "\r\n" to find a
complete IRC line), that loop can block forever on a slow or malicious
client — freezing the ENTIRE server, since it's single-threaded.

fcntl(fd, F_SETFL, O_NONBLOCK) makes every read/accept call on that fd
return immediately, even with no data (returns -1, errno = EAGAIN).
This means poll() stays the single source of truth for "try again later" —
no syscall can ever freeze the program.

Rule: set O_NONBLOCK on _serverSocket right after socket(), and on every
client fd right after accept() — before writing any recv() logic.



## recv() prototype :
```cpp
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```
**sockfd** = The socket we want to read from.                                
**buf** = A memory area where Linux will copy the received bytes.            
**len** = Maximum number of bytes we want.          
**flags** = Extra options. (0 means no special flags are applied)

## recv() returns three different cases, not two :
```cpp
int n = recv(fd, buffer, sizeof(buffer), 0);
```
- ```n > 0``` → got ```n``` bytes of actual data.
- ```n == 0``` → **The client disconnected**.  client closed the connection cleanly (you should ```close(fd)``` and remove it from ```_pollfds```).
- ```n < 0``` → an actual error occurred.



## Why we must both close(fd) AND erase it from _pollfds

These are two separate problems, and skipping either one causes a different bug.

**```close(fd)``` — releases the resource at the OS level**

A file descriptor is a finite resource the kernel hands out per process.
When a client disconnects (```recv()``` returns 0) or we decide to kick them,
the socket is still technically open from the kernel's point of view
until we explicitly call ```close(fd)```. If we never close it, that ```fd``` stays
allocated forever — a slow leak. With enough disconnects/reconnects
during testing or the eval, we'd eventually hit the ```OS``` limit on open
file descriptors and ```accept()``` would start failing for brand new clients,
even though the old ones are long gone.

**```erase``` from ```_pollfds``` — releases it at our program's level**

```close(fd)``` only tells the ```kernel``` "I'm done with this." It does NOT
remove the entry from our ```_pollfds``` vector. If we ```close()``` a ```fd``` but
leave it in ```_pollfds```, two things go wrong:

1. ```poll()``` is still watching a closed fd. Depending on timing, this can
   make ```poll()``` return ```POLLNVAL``` (invalid fd) or ```POLLHUP``` repeatedly, and
   our loop has no client to actually talk to anymore — recv() on a
   closed fd is undefined behavior we don't want to deal with.

2. Worse: fds get ```RECYCLED```. If client A disconnects (```fd``` 5 closed) and
   we forget to erase it from ```_pollfds```, then client B connects and the
   kernel reuses fd 5 for them, we now have a stale leftover entry that
   either duplicates fd 5 in our vector, or causes us to treat client B's
   data as if it belongs to client A (who doesn't exist anymore).

**The rule**

Every disconnect must do both, together, every time:
  1. ```close(fd)```                         -> tell the ```kernel``` we're done
  2. erase the matching pollfd entry   -> tell OUR loop we're done

Doing only one half leaves either a ```kernel-level``` leak or a
program-level ghost entry that can crash or misbehave later.


# -----------------------------------------------------------------------------------


# Why removing disconnected clients from `_pollfds` is necessary

Suppose our server is currently monitoring the following file descriptors:

```text
_pollfds:

fd=3
fd=4
fd=5
```

Where:

```text
fd=3 -> listening socket
fd=4 -> client #1
fd=5 -> client #2
```

---

Now imagine client #1 disconnects.

We call:

```cpp
close(4);
```

This closes the socket in the kernel.

After this call:

```text
fd=4 no longer exists
```

The connection is gone.

---

However, if we forget to remove it from `_pollfds`, the vector still contains:

```text
_pollfds:

fd=3
fd=4
fd=5
```

Even though `fd=4` has already been closed.

---

Later, the server calls:

```cpp
poll(_pollfds.data(), _pollfds.size(), -1);
```

This tells the kernel:

> Monitor all file descriptors currently stored inside `_pollfds`.

The problem is that `fd=4` is still present in the vector.

The kernel is therefore asked to monitor a file descriptor that no longer exists.

This can result in:

```text
POLLNVAL
```

which means:

```text
Invalid file descriptor
```

and may lead to unexpected behavior or errors inside the event loop.

---

For this reason, when a client disconnects we must perform **two separate actions**:

### 1. Close the socket

```cpp
close(clientFd);
```

This removes the file descriptor from the kernel.

---

### 2. Remove it from `_pollfds`

```cpp
_pollfds.erase(...);
```

This removes the file descriptor from the list being monitored by `poll()`.

---

Both operations are required.

Closing the socket is not enough.

Removing it from `_pollfds` is not enough.

The client must be removed from **both the kernel and the poll monitoring list**.

# -----------------------------------------------------------------------------------



# problen to fix later !!
One more thing (important)

There's actually another bug hiding in your poll loop.

Imagine:

_pollfds:

fd=3
fd=4
fd=5

You're inside:

for (size_t i = 0; i < _pollfds.size(); i++)

and during iteration:

removeClient(fd=4);

does:

erase(...)

which changes the vector size while you're iterating.

That can cause elements to be skipped.

We're not going to solve it yet because I first want you to implement removeClient() and see the behavior.

Then we'll discuss the correct way to handle erasing during iteration.

# -----------------------------------------------------------------------------------


# ```_pollfds``` vs ```_clients```:

## ```_pollfds```

Its job is ONLY:

Which file descriptors should ```poll()``` watch?

Example:
```cpp
_pollfds

fd=3
fd=4
fd=5
```
It knows nothing about nicknames or usernames.


## ```_clients```

Its job is:

Give me the object representing this connected client.

Example:
```cpp
_clients

4 ───► Client(fd=4)
5 ───► Client(fd=5)
```
It knows nothing about ```poll()```.



These two containers complement each other.
```cpp
_pollfds
        ↓
Tells us WHICH fd has an event

_clients
        ↓
Lets us retrieve everything about that client
```
This separation is very common in event-driven servers.

```cpp
                Server
                   │
        ┌──────────┴──────────┐
        │                     │
   _pollfds              _clients
        │                     │
        │                     │
 poll() watches        Stores Client objects
        │                     │
        └──────────┬──────────┘
                   │
              client fd           
```

See how each container has a single responsibility?

-  ```_pollfds``` answers **"Which fds should poll monitor?"**
- ```_clients``` answers **"Who is client 4?"**

That's clean architecture.

# -----------------------------------------------------------------------------------

## Why we can't use [] to loop a map — and what iterators are

### First, why can't we loop a map like a vector?

With a vector we can write:

```cpp
for (size_t i = 0; i < _pollfds.size(); i++)
{
    std::cout << _pollfds[i].fd << std::endl;
}
```

Because a vector is like an array — everything is stored next to each
other in memory:

```
+-----+-----+-----+-----+
| fd3 | fd4 | fd5 | fd6 |
+-----+-----+-----+-----+
  [0]   [1]   [2]   [3]
```

So `_pollfds[2]` means "give me what's at position 2." Simple math.

A map is NOT an array. Internally it's built as a **Red-Black Tree**
(a self-balancing binary search tree). We don't need to understand the
algorithm — what matters is that the nodes are **scattered in memory**,
not sitting next to each other:

```
          5
         / \
        3   8
       /     \
      1       9
```

Each node lives at a completely different memory address:

```
Node 5 -> 0x1000
Node 3 -> 0x4A80
Node 8 -> 0x2200
```

So `_clients[0]` makes no sense — there is no "position 0" or "first
memory slot." The map has no index system, only keys.

---

### Then how do we move through the map?

Imagine you're reading a book. You don't know the memory address of
page 72. You just hold a **bookmark** that says "I'm currently here."

That bookmark is an **iterator**.

> An iterator is an object that points to one element in a container
> and knows how to move to the next one.

Not exactly a pointer, but close enough to think of it that way.

---

### Visualizing an iterator step by step

Suppose our map contains:

```
4 -> Client A
7 -> Client B
9 -> Client C
```

We declare an iterator:

```cpp
std::map<int, Client>::iterator it;
```

At this point `it` points nowhere. Then:

```cpp
it = _clients.begin();
```

This places the bookmark on the **first** element:

```
it
↓
4 -> Client A
7 -> Client B
9 -> Client C
```

---

### What is begin()?

```cpp
_clients.begin()
```

"Open the book. Put your finger on the first page."
Returns an iterator pointing to the first element in the map.

---

### What is end()?

This confuses everyone at first. People assume `end()` points to the
last element. It does NOT.

`end()` points to **one step past the last element** — an imaginary
position that doesn't hold any real data:

```
4 -> Client A
7 -> Client B
9 -> Client C
(end) <- end() points here, not at 9
```

This is why we write `it != _clients.end()` as our loop condition —
it means "keep going as long as we haven't walked off the end."

Trying to dereference `end()` (reading `it->first` or `it->second`
when `it == end()`) is undefined behavior and will likely crash.

---

### The full loop

```cpp
for (std::map<int, Client>::iterator it = _clients.begin();
     it != _clients.end();
     ++it)
{
    int    fd     = it->first;   // the key   (the client's fd)
    Client &client = it->second; // the value (the Client object)

    std::cout << "fd=" << fd << " nick=" << client.getNickname() << std::endl;
}
```

Breaking it down:

| part | meaning |
|---|---|
| `it = _clients.begin()` | place the bookmark on the first element |
| `it != _clients.end()` | stop when we've gone past the last element |
| `++it` | move the bookmark to the next element |
| `it->first` | the key (int fd) |
| `it->second` | the value (Client object) |

---

### One-line mental model

A vector uses indexes because its elements are next to each other in
memory. A map uses iterators because its elements are scattered — the
iterator is the only way to walk from one node to the next without
knowing where anything lives in memory.


# -------------------------------------------------------------------------------------

## Understanding find()

### What find() does

`find()` searches the map for a specific key and returns an iterator
pointing to it — or tells you it wasn't found.

Suppose our map contains:

```
_clients

4 -> Client(4)
5 -> Client(5)
8 -> Client(8)
```

We write:

```cpp
_clients.find(5);
```

The map searches for `key = 5`. Two possible outcomes:

**Key EXISTS** — returns an iterator pointing to that entry:

```
it
↓
5 -> Client(5)
```

**Key DOES NOT EXIST** — returns `_clients.end()`:

```
4 -> Client(4)
5 -> Client(5)      <- not here
8 -> Client(8)
(end) <- returns this instead
```

---

### Why end() means "not found"

Remember from the iterator section: `end()` doesn't point to the last
element — it points to an imaginary position one step past it, which
holds no real data.

The STL reuses this position as its "nothing was found" signal.
Whenever you call `find()` and the key doesn't exist, it hands you
back `end()` — which you already know means "not a valid element."

That's why the standard check is always:

```cpp
std::map<int, Client>::iterator it = _clients.find(fd);
if (it != _clients.end())
{
    // found — it->first is the fd, it->second is the Client
}
else
{
    // not found
}
```

---

### Why not just return NULL?

Because iterators are not pointers — they are objects. You can't set
an object to NULL the way you would a raw pointer. The STL needed a
universal "nothing here" value that works for every container type,
and `end()` is exactly that: a valid iterator object that every
container already has, which by definition points to no real element.

So the STL made a convention:

> `end()` = "I searched and found nothing."

Every STL container uses this same convention — `find()` on a
`std::map`, `std::set`, or any other container always returns `end()`
on failure, never NULL.

---

### find() vs [] — the key difference

```cpp
// find() — safe, never creates anything
std::map<int, Client>::iterator it = _clients.find(fd);
if (it != _clients.end())
    it->second.doSomething();   // we KNOW it exists before touching it

// [] — dangerous if fd might not exist
_clients[fd].doSomething();     // silently creates a ghost Client if fd is missing
```

Use `find()` any time you're not 100% certain the key exists.
Use `[]` only when you already know the key is there, or you
explicitly want to create a new entry.