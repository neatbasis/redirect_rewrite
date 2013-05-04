# redirect_rewrite

A simple [redirector helper](http://wiki.squid-cache.org/Features/Redirectors) for Squid proxy

Uses regex to find a redirection url and returns the destination url to Squid
It was made for converting Google and Facebook redirection URLs to their actual destination URLs in order to get rid of the redirection delay caused by waiting for response from Google or Facebook servers

Add your own matchers in redirect_rewrite.c or use it as it is.

Build it like this:

```bash
make
```

Copy redirect_rewrite somewhere like /usr/local/bin/ or use `make install`

```bash
make install
```

Modify your squid.conf to contain the path of the executable

```bash
url_rewrite_program /path/to/redirect_rewrite
```

