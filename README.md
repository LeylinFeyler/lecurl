# lecurl (C)

A minimal `curl`-like HTTP client written in C.

* Supports `GET` and `HEAD` requests.
* Can include headers with `-i`.
* Can save output to file with `-o`.

---

## Build

Compile the project:

```bash
make
```

---

## Run

Basic usage:

```bash
./lecurl http://example.com        # GET request
./lecurl -i http://example.com     # GET with headers
./lecurl -I http://example.com     # HEAD request
./lecurl -o page.html http://example.com  # Save output to file
```

---

## Notes

* Now its only supports HTTP/1.0 and 1.1 (no HTTPS).
