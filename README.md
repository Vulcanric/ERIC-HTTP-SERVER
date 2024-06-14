# Eric Hypertext Transfer Protocol Server
![eric web server icon](https://github.com/Vulcanric/ERIC-HTTP-SERVER/blob/main/img/eric%20webserver.png)

**eric** is a multiprocessing and a multiclient http web server, capable of interacting with web clients, receiving request, retrieving data and serving request with a response, all based on the HTTP protocol.
## The story behind
This project was envisioned as way to challenge myself (I love doing things that are bigger than me :). For this, I went through days and weeks of learning the C socket API, implementing similar [projects](https://github.com/Vulcanric/Network_Programming) as to get prepared for this one. I could call those projects **version 1**, only that they aren't http-based.
With all optimism, I proceeded to create a minimalist web server first, one that had a fixed response to every request. This was only to test if it works, and it did. Here is the first time my web server worked.
![The first time my webserver worked](https://github.com/Vulcanric/ERIC-HTTP-SERVER/blob/main/img/THE%20FIRST%20time%20my%20web%20server%20started%20working.jpg)

And therefore, with some challenges I passed through, and the victory I got over them, I was able to create a fully functional web server.

For a full story of how I created this server, including failures and how I overcame them, check out my blog: [**How I created a web server**](https://dev.to/vulcanric/how-i-created-a-web-server-for-my-portfolio-3j7e).

## Installation
Installation of this software includes just _ steps
1. Clone this repository as a viewer
```bash
git clone https://github.com/Vulcanric/ERIC-HTTP-SERVER.git
```
2. Enter into the project folder using cd
```bash
cd ERIC-HTTP-SERVER
```
3. Compile source code
```bash
gcc *.c -o eric
```
4. Run executable
```bash
./eric
```
#### Note: that the web server will keep running according to the default information provided by the configuration file, unless you alter it

## Usage
