# Toraliser

Toraliser is a lightweight shared library that transparently routes network traffic through the Tor SOCKS4 proxy. By intercepting the `connect()` system call, it automatically redirects TCP connections through Tor without modifying the application source code.

## Why Toraliser?

Many applications do not natively support Tor or SOCKS proxies. Toraliser solves this problem by:

- **Transparent proxying**: No need to modify application code or configure proxy settings
- **Simple integration**: Just preload the library with `LD_PRELOAD`
- **SOCKS4 protocol support**: Compatible with Tor's SOCKS4 proxy implementation

## How It Works

Toraliser uses the `LD_PRELOAD` mechanism to intercept the standard `connect()` function. When an application attempts to establish a TCP connection:

1. Toraliser creates a new socket
2. Connects to the local Tor SOCKS4 proxy (default: 127.0.0.1:9050)
3. Sends a SOCKS4 handshake with the destination address
4. Verifies the proxy response
5. Duplicates the proxy socket to the original socket file descriptor

## Requirements

- **GCC**: For compiling the shared library
- **Tor**: Running Tor service with SOCKS4 proxy enabled (default port 9050)
- **Linux**: Uses Linux-specific features (dlopen, dlsym)
- **Development libraries**:
  - `libdl` (for dynamic linking)
  - Standard C library headers

## Building

Compile the shared library using the provided Makefile:

```bash
make
```

This will generate `Toraliser.so` in the current directory.

To clean build artifacts:

```bash
rm -rf Toraliser.so
```

## Usage

### Basic Usage

Preload the library when running any network application:

```bash
LD_PRELOAD=./Toraliser.so ./your_application
```

### Example:

```bash
# Without Tor (shows your real IP)
curl https://check.torproject.org

# With Tor (shows Tor exit node IP)
LD_PRELOAD=./Toraliser.so curl https://check.torproject.org
```

### Example: Testing with wget

```bash
LD_PRELOAD=./Toraliser.so wget https://example.com
```

## Configuration

You can modify the proxy settings by editing the constants in `Toraliser.h`:

```c
#define tor_proxy_addr "127.0.0.1"  // Tor proxy address
#define tor_proxy_port 9050          // Tor proxy port
#define Username "anonyms"          // SOCKS4 username (optional)
```

After modifying, recompile with `make`.

## Error Codes

- `-2`: Socket creation failed
- `-3`: Connection to Tor proxy server failed
- `-4`: SOCKS4 handshake failed

## Limitations

- **SOCKS4 only**: Currently supports SOCKS4 protocol ONLY
- **TCP only**: Only works with TCP connections (not UDP)
- **IPv4 only**: Does not support IPv6 addresses

## Security Considerations

- Ensure Tor is properly configured and running before using Toraliser
- Verify DNS leaks are not occurring (SOCKS4 does not handle DNS resolution)
- Use with applications that don't implement their own TLS certificate pinning

## Troubleshooting

### Connection fails with "Failed to connect the socket to the tor proxy server"
- Ensure Tor is running: `sudo systemctl status tor` or `tor`
- Verify Tor is listening on port 9050: `netstat -tlnp | grep 9050`
- Check if Tor SOCKS4 is enabled in torrc: `SocksPort 9050`

### Handshake fails
- Verify the destination address is reachable through Tor
- Check Tor logs for connection issues
- Ensure the application is using TCP (not UDP)

### Library not loading
- Verify the path to `Toraliser.so` is correct
- Check file permissions: `ls -l Toraliser.so`
- Use absolute path if relative path fails

## Project Structure

```
Toraliser/
├── Toraliser.h      # Header file with structures and declarations
├── Toraliser.c      # Main implementation with connect() interception
├── Makefile         # Build configuration
└── README.md        # This file
```

## Contributing

Contributions are welcome! Please ensure code follows the existing style and compiles without warnings.

## Disclaimer

Toraliser is intended for legitimate privacy and security purposes. Users are responsible for complying with applicable laws and regulations. The authors are not responsible for misuse of this software.
