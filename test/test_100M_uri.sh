# This shell test was written in a similar form by psef15 @ SS/14

cd "$( dirname "${BASH_SOURCE[0]}" )"/../htdocs && pwd
echo "GET with uri of size: 104857600"
python -c "print('GET /index.html' + 'a' * 104857600 + ' HTTP/1.1\r\nHost: extern\r\n\r\n')" > bigRequest.dat
cat bigRequest.dat | nc localhost 31337
rm bigRequest.dat
