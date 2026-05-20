# lecurl (C)

A small `curl`-like HTTP client written in C using raw sockets.

Features:

* `GET`, `HEAD` and custom HTTP methods
* Custom headers
* Request body support
* Basic authentication
* Retry and timeout options
* Save response to file
* Chunked transfer decoding

---

## Build

```bash
make
```

---

## Run

Basic usage:

```bash
./lecurl <url>
```

Example:

```bash
./lecurl http://example.com
```

---

## Usage

```bash
./lecurl [options] <url>
```

Options:

```text
-o <file>            write output to file
-I                   HEAD request
-i                   include response headers
-X <method>          custom HTTP method
-d <data>            request body
-H <header>          custom header
-u <user:pass>       basic auth
-L                   follow redirects
--connect-timeout N  connection timeout
--retry N            retry count
-h                   show help
```

---

## Notes

* Currently supports HTTP/1.0 and HTTP/1.1
* HTTPS is not supported yet
