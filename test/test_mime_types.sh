# This shell test was written in a similar form by psef15 @ SS/14

echo "GET /images/aaaaaaa..."
echo "Content-Type: image/png expected"
printf "GET /images/aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa.png HTTP/1.1\r\nHost: extern\r\n\r\n" | nc localhost 31337 | grep --text Content-Type:
echo "------------------------------------------------------------"

echo "GET /images/ein%20leerzeichen.png"
echo "Content-Type: image/png expected"
printf "GET /images/ein%%20leerzeichen.png HTTP/1.1\r\nHost: extern\r\n\r\n" | nc localhost 31337 | grep --text Content-Type:
echo "------------------------------------------------------------"

echo "GET /images/tux"
echo "Content-Type: application/octet-stream expected"
printf "GET /images/tux HTTP/1.1\r\nHost: extern\r\n\r\n" | nc localhost 31337 | grep --text Content-Type:
echo "------------------------------------------------------------"

echo "GET /images/tux."
echo "Content-Type: application/octet-stream expected"
printf "GET /images/tux. HTTP/1.1\r\nHost: extern\r\n\r\n" | nc localhost 31337 | grep --text Content-Type:
echo "------------------------------------------------------------"

echo "GET /images/tux..pdf.JPg"
echo "Content-Type: image/jpeg expected"
printf "GET /images/tux..pdf.JPg HTTP/1.1\r\nHost: extern\r\n\r\n" | nc localhost 31337 | grep --text Content-Type:
echo "------------------------------------------------------------"

echo "GET /images/tux.jpg"
echo "Content-Type: image/jpeg expected"
printf "GET /images/tux.jpg HTTP/1.1\r\nHost: extern\r\n\r\n" | nc localhost 31337 | grep --text Content-Type:
echo "------------------------------------------------------------"

echo "GET /images/tux.png"
echo "Content-Type: image/png expected"
printf "GET /images/tux.png HTTP/1.1\r\nHost: extern\r\n\r\n" | nc localhost 31337 | grep --text Content-Type:
echo "------------------------------------------------------------"

echo "GET /images/tux.xxx.jPG"
echo "Content-Type: image/jpeg expected"
printf "GET /images/tux.xxx.jPG HTTP/1.1\r\nHost: extern\r\n\r\n" | nc localhost 31337 | grep --text Content-Type:
echo "------------------------------------------------------------"

echo "GET /images/TUX1.PNG"
echo "Content-Type: image/png expected"
printf "GET /images/TUX1.PNG HTTP/1.1\r\nHost: extern\r\n\r\n" | nc localhost 31337 | grep --text Content-Type:
echo "------------------------------------------------------------"

echo "GET /index.html"
echo "Content-Type: text/html expected"
printf "GET /index.html HTTP/1.1\r\nHost: extern\r\n\r\n" | nc localhost 31337 | grep --text Content-Type:
echo "------------------------------------------------------------"

echo "GET /test"
echo "Content-Type: application/octet-stream expected"
printf "GET /test HTTP/1.1\r\nHost: extern\r\n\r\n" | nc localhost 31337 | grep --text Content-Type:
echo "------------------------------------------------------------"

echo "GET /js/javascript.js"
echo "Content-Type: text/javascript expected"
printf "GET /js/javascript.js HTTP/1.1\r\nHost: extern\r\n\r\n" | nc localhost 31337 | grep --text Content-Type:
echo "------------------------------------------------------------"
