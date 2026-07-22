too few params        → :server 461 yournick INVITE :Not enough parameters
target doesn't exist  → :server 401 yournick targetnick :No such nick
channel doesn't exist → :server 403 yournick #channel :No such channel
you're not in channel → :server 442 yournick #channel :You're not on that channel
you're not operator   → :server 482 yournick #channel :You're not channel operator
target already in     → :server 443 yournick targetnick #channel :is already on channel

this is to the target :
:yournick!user@host INVITE targetnick :#channel
this is to the sender :
:server 341 yournick targetnick #channel