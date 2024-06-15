# Eric Hypertext Transfer Protocol Server
---
<center><img src=https://github.com/Vulcanric/ERIC-HTTP-SERVER/blob/main/.img/eric%20webserver.png></img></center>

**eric** is a multiprocessing and a multiclient http web server, capable of interacting with web clients, receiving request, retrieving data and serving request with a response, all based on the HTTP protocol.
## The story behind
---
This project was envisioned as way to challenge myself (as I love doing things that are bigger than me :). So I went on for days learning the C socket API, implementing similar [projects](https://github.com/Vulcanric/Network_Programming) as to get prepared for this one. I could call those projects **version 1**, only that they aren't http-based.
With all optimism, I proceeded to create a minimalist web server first, one that had a fixed response to every request. This was only to test if it works, and it did. Here is the first time my web server worked:

![The first time my webserver worked](https://github.com/Vulcanric/ERIC-HTTP-SERVER/blob/main/.img/THE%20FIRST%20time%20my%20web%20server%20started%20working.jpg)
<hr>

And therefore, with some challenges I passed through, and how I was able to overcome them, I was able to create a fully functional web server.

For a full story of how I created this server, including failures and how I was able to overcome them, check out my blog post:<br>[**How I created a web server**](https://dev.to/vulcanric/how-i-created-a-web-server-for-my-portfolio-3j7e).

## Demo
---
Here's is eric in action
[![server icon](https://github.com/Vulcanric/ERIC-HTTP-SERVER/blob/main/.img/thumbnail%20for%20eric%20server.jpg)](https://vimeo.com/956574620)
## Installation
---
Installation of this software includes just 3 steps
1. Clone this repository as a viewer
```bash
$ git clone https://github.com/Vulcanric/ERIC-HTTP-SERVER.git
```
2. Enter into the project folder using cd
```bash
$ cd ERIC-HTTP-SERVER
```
3. Compile source code
```bash
$ gcc *.c -o eric
```
**There'll be a more sound mode of installation in the future.**

## Usage
---
You could use this web server with the steps provided below
1. Open the configuration file `eric_/eric.conf` using your favorite text editor
2. Edit this file to your needs, as it controls the way the web server behaves
3. start or restart the web server to get the latest settings as specified in `eric_/eric.conf`
