# ft_irc Testing Checklist

This document contains edge cases and protocol tests that should be performed before considering the IRC server complete.

---

# Registration

## PASS

- [ ] PASS before NICK/USER.
- [ ] PASS with wrong password.
- [ ] PASS with no parameter.
- [ ] PASS sent twice.
- [ ] PASS after registration.

---

## NICK

- [ ] Valid nickname.
- [ ] Empty nickname.
- [ ] Nickname already in use.
- [ ] Nickname containing invalid characters.
- [ ] Change nickname after registration.
- [ ] Change nickname to the current nickname.
- [ ] Change nickname while inside several channels.
- [ ] Other clients receive nickname change.

---

## USER

- [ ] USER before PASS.
- [ ] USER before NICK.
- [ ] USER with missing parameters.
- [ ] USER sent twice.
- [ ] USER after registration.

---

## Registration order

- [ ] PASS -> NICK -> USER
- [ ] PASS -> USER -> NICK
- [ ] NICK -> PASS -> USER
- [ ] USER -> PASS -> NICK
- [ ] USER -> NICK -> PASS
- [ ] NICK -> USER -> PASS

---

# JOIN

## Basic

- [ ] Join existing channel.
- [ ] Join non-existing channel (should create it).
- [ ] First member becomes operator.
- [ ] Second member is not operator.
- [ ] Join several channels at once (#a,#b,#c).
- [ ] JOIN 0 leaves every channel.

---

## Invalid joins

- [ ] JOIN with no parameter.
- [ ] JOIN invalid channel name.
- [ ] JOIN channel already joined.
- [ ] JOIN empty channel name.
- [ ] JOIN "#a,,#b".
- [ ] JOIN "#a,".
- [ ] JOIN ",#a".

---

## Invite-only

- [ ] Join invite-only without invite.
- [ ] Join invite-only after invite.
- [ ] Invitation disappears after successful join.

---

## Password

- [ ] Correct password.
- [ ] Wrong password.
- [ ] Missing password.
- [ ] Password on channel without +k.

---

## User limit

- [ ] Join when full.
- [ ] Join when one place remains.
- [ ] Disable limit then join.

---

# PART

- [ ] PART existing channel.
- [ ] PART channel not joined.
- [ ] PART non-existing channel.
- [ ] PART multiple channels.
- [ ] PART with reason.
- [ ] PART without reason.
- [ ] PART last user (channel removed).
- [ ] Operator leaves while others remain.
- [ ] PART after already parted.

---

# PRIVMSG

## User

- [ ] User -> User.
- [ ] User -> Self.
- [ ] User -> Unknown nick.

---

## Channel

- [ ] Message to joined channel.
- [ ] Message to channel not joined.
- [ ] Empty message.
- [ ] Missing target.
- [ ] Missing text.
- [ ] Multiple consecutive messages.
- [ ] Very long message.

---

# NOTICE

- [ ] NOTICE user.
- [ ] NOTICE channel.
- [ ] NOTICE invalid target.
- [ ] NOTICE with missing parameters.
- [ ] Verify no automatic error replies.

---

# INVITE

- [ ] Invite existing user.
- [ ] Invite unknown user.
- [ ] Invite user already in channel.
- [ ] Invite user twice.
- [ ] Invite while not operator.
- [ ] Invite while not on channel.
- [ ] Invite to unknown channel.
- [ ] Invite yourself.

---

# KICK

- [ ] Kick member.
- [ ] Kick unknown user.
- [ ] Kick non-member.
- [ ] Kick while not operator.
- [ ] Kick yourself.
- [ ] Kick with reason.
- [ ] Kick without reason.
- [ ] Kick last member.

---

# TOPIC

- [ ] Read topic.
- [ ] Set topic.
- [ ] Clear topic.
- [ ] Change topic repeatedly.
- [ ] Change topic without operator (+t).
- [ ] Change topic with operator.
- [ ] Read topic on channel without topic.

---

# MODE

## Invite-only (+i)

- [ ] +i
- [ ] -i
- [ ] Repeat +i
- [ ] Repeat -i

---

## Topic (+t)

- [ ] +t
- [ ] -t

---

## Password (+k)

- [ ] +k password
- [ ] +k without password.
- [ ] Change password.
- [ ] -k

---

## Limit (+l)

- [ ] +l valid number.
- [ ] +l missing number.
- [ ] +l zero.
- [ ] +l negative.
- [ ] +l non-number.
- [ ] -l

---

## Operator (+o)

- [ ] +o existing member.
- [ ] +o non-member.
- [ ] +o unknown nick.
- [ ] -o operator.
- [ ] -o last operator.
- [ ] +o already operator.
- [ ] -o non-operator.

---

## General MODE

- [ ] MODE unknown channel.
- [ ] MODE while not operator.
- [ ] MODE unknown flag.
- [ ] MODE with missing parameters.

---

# NAMES

- [ ] NAMES existing channel.
- [ ] NAMES empty channel.
- [ ] NAMES unknown channel.
- [ ] NAMES multiple channels.
- [ ] Verify operators have '@'.

---

# LIST

- [ ] LIST all channels.
- [ ] LIST one channel.
- [ ] LIST unknown channel.
- [ ] Empty server.

---

# WHO

- [ ] WHO existing channel.
- [ ] WHO unknown channel.
- [ ] WHO user.
- [ ] WHO with no parameter.

---

# WHOIS

- [ ] WHOIS existing user.
- [ ] WHOIS unknown user.
- [ ] WHOIS yourself.

---

# PING/PONG

- [ ] PING.
- [ ] Multiple PINGs.
- [ ] Empty PING.

---

# QUIT

- [ ] QUIT with reason.
- [ ] QUIT without reason.
- [ ] QUIT while in one channel.
- [ ] QUIT while in multiple channels.
- [ ] QUIT as operator.
- [ ] QUIT last user in channel.
- [ ] Other users receive QUIT.

---

# Channel lifecycle

- [ ] Empty channel gets deleted.
- [ ] Recreate deleted channel.
- [ ] First member becomes operator again.

---

# Multiple clients

- [ ] 2 clients chatting.
- [ ] 5 clients chatting.
- [ ] Join/leave simultaneously.
- [ ] Nick change while messaging.
- [ ] Kick while sending messages.

---

# Robustness

- [ ] Empty line.
- [ ] Unknown command.
- [ ] Lowercase commands.
- [ ] Mixed-case commands.
- [ ] Commands with extra spaces.
- [ ] Very long nickname.
- [ ] Very long channel name.
- [ ] Very long topic.
- [ ] Flood with many commands.
- [ ] Disconnect without QUIT.
- [ ] Reconnect after disconnect.

---

# HexChat compatibility

- [ ] Channel appears after JOIN.
- [ ] Channel disappears after PART.
- [ ] Channel disappears after QUIT.
- [ ] Nick changes immediately.
- [ ] Topic updates immediately.
- [ ] MODE updates immediately.
- [ ] KICK updates immediately.
- [ ] INVITE works through GUI.
- [ ] Multiple JOIN (#a,#b,#c) opens separate tabs.
- [ ] Rejoin after PART.
- [ ] Rejoin after QUIT.

---

# Bonus Bot

- [ ] help
- [ ] time
- [ ] coin
- [ ] joke
- [ ] quote
- [ ] fact
- [ ] calc
- [ ] Invalid bot command.
- [ ] Bot while user not registered.
- [ ] Bot command with missing parameters.