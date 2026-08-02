*This project has been created as part of the 42 curriculum by **anaamaja**, **rmaanane**, **hfakou**.*

# ft_irc

## Description

**ft_irc** is a simplified Internet Relay Chat (IRC) server developed in **C++98** as part of the 42 curriculum. The objective of the project is to understand network programming by implementing an IRC server capable of communicating with standard IRC clients such as **HexChat**.

The server uses TCP sockets with non-blocking I/O and `poll()` to manage multiple clients simultaneously without using threads or processes. It supports user registration, channel management, messaging, and the operator commands required by the project subject.

### Features

- Multi-client TCP server
- Non-blocking sockets using `poll()`
- Password authentication
- User registration (`PASS`, `NICK`, `USER`)
- Channel creation and management
- Private and channel messaging
- Channel operators
- Channel modes (`+i`, `+t`, `+k`, `+o`, `+l`)
- Compatible with HexChat

---

# Instructions

## Requirements

- C++98 compatible compiler
- GNU Make
- Linux or macOS

## Compilation

```bash
make
```

## Run

```bash
./ircserv <port> <password>
```

Example:

```bash
./ircserv 6667 password
```

## Connecting with HexChat

Create a new network in HexChat and connect to:

```
Address : localhost/6667
Password: password
```

HexChat will automatically complete the registration.

## Manual testing

Using netcat:

```bash
nc -C localhost 6667
```

Example:

```text
PASS password
NICK user
USER user 0 * :Real Name
JOIN #general
PRIVMSG #general :Hello!
```

---

# Supported Commands

- PASS
- NICK
- USER
- JOIN
- PART
- PRIVMSG
- NOTICE
- INVITE
- NAMES
- KICK
- TOPIC
- MODE
- WHO
- LIST
- QUIT

---

# Resources

## Documentation

- RFC 1459 — Internet Relay Chat Protocol
- Beej's Guide to Network Programming
- Linux man pages (`socket`, `poll`, `recv`, `send`, `fcntl`)

## Testing

- HexChat
- netcat (nc)

## AI Usage

AI was used as a learning and debugging assistant during the project.

It was mainly used for:

- Understanding the IRC protocol (RFC 1459)
- Clarifying C++98 concepts
- Understanding socket programming and `poll()`
- Debugging protocol and HexChat compatibility issues
- Reviewing edge cases and IRC numeric replies

All implementation, design decisions, testing, and final code were completed and validated by the project team.