# ft_irc

## Code an irc server in C++, compatible with a commercial client (weechat) with user operators and channel management (ban, private channels, invite only, etc...)
## This project was made with [victor-chevillotte](https://github.com/victor-chevillotte)


Skills:
* sockets
* client/server communication
* understanding RFC

brew install weechat

useful commands to test this project :

```
nc -c (ends line with /r/n) 127.0.0.1  (server address) 6697 (port)
nc -c 127.0.0.1 6697

suspend : CTRL+Z
resume : fg

see options in weechat :
/fset
/set

change username in weechat :
/set irc.server.ft_irc.nicks "john"
/set irc.server.ft_irc.username "john"

open another weechat in subfolder :
weechat -d [subfolder]

add ft_irc in weechat's server list :
/server add ft_irc 127.0.0.1/6697

connection to server with password :
/connect ft_irc -password=****

give operator status
/mode #channel +o #nick

weechat buffer navigation :
Alt+Down / Alt+Up

Sending files with DCC protocol :
/dcc send nickname filepath 
then press 'a' to accept the request in the xfer buffer on the target client
the file is downloaded in the xfer subfolder

Modes :
        o - give/take channel operator privilege;
        v - give/take the voice privilege;

        a - toggle the anonymous channel flag;
        i - toggle the invite-only channel flag;
        m - toggle the moderated channel;
        n - toggle the no messages to channel from clients on the outside;
        q - toggle the quiet channel flag;
        p - toggle the private channel flag;
        s - toggle the secret channel flag;
        t - toggle the topic settable by channel operator only flag;

        k - set/remove the channel key (password);
        l - set/remove the user limit to channel;

        b - set/remove ban mask to keep users out;
        e - set/remove an exception mask to override a ban mask;
        I - set/remove an invitation mask to automatically override the invite-only flag;
```
