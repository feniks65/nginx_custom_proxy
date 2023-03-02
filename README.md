# Nginx Custom Proxy Module

This project is a custom Nginx module written in C++ that acts as a reverse proxy with additional features such as request header modification and custom logging. The module is built using CMake and can be dynamically loaded into an existing Nginx installation.

## Features

- **Reverse Proxy:** Forwards client requests to an upstream server.
- **Header Modification:** Adds or modifies specific headers before forwarding the request.
- **Custom Logging:** Logs detailed information about the request and response.

## Prerequisites

- **Nginx Source Code:** The module requires access to the Nginx source code for compilation.
- **CMake:** Make sure you have CMake installed on your system.
- **C++ Compiler:** A C++11 compatible compiler (e.g., g++).

## Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/feniks65/nginx_custom_proxy.git
cd nginx_custom_proxy
```

### 2. Modify Nginx Source Path
```bash
set(NGINX_SRC_PATH "/path/to/nginx/src")
```

### 3. Build the module
```bash
mkdir build
cd build
cmake ..
make
```

### 4. Configure Nginx
nginx.conf
```bash
load_module /path/to/ngx_http_custom_proxy_module.so;

http {
    server {
        listen 80;

        location /proxy {
            custom_proxy_pass http://your-upstream-server;
        }
    }
}
```
