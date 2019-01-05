while :
do
random=$(head -c 31 /dev/urandom | base64 )
printf "GET / HTTP/1.1\r\nHost: intern\r\nAuthorization: Basic "${random}"\r\n\r\n" | nc localhost 31337 > /dev/null 2>&1
done
