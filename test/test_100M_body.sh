# This shell test was written in a similar form by psef15 @ SS/14

cd "$( dirname "${BASH_SOURCE[0]}" )"/../htdocs && pwd
echo "GET /index.html with a body of size: 104857600"
python -c "print('GET /index.html HTTP/1.1\r\nContent-Length: 104857600\r\nHost: extern\r\n\r\n' + 'a' * 104857600)" > bigRequest.dat
cat bigRequest.dat | nc localhost 31337
rm bigRequest.dat
