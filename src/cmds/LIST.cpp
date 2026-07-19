#include "../../include/Server.hpp"
/*
**
** The list message is used to list channels and their topics.  If  the
** <channel>  parameter  is  used,  only  the  status  of  that  channel
** is displayed.  Private  channels  are  listed  (without  their
** topics)  as channel "Prv" unless the client generating the query is
** actually on that channel.  Likewise, secret channels are not listed
**
*/
