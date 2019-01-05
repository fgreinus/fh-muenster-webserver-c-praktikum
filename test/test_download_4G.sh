# This shell test was written in a similar form by psef15 @ SS/14

cd "$( dirname "${BASH_SOURCE[0]}" )"/../htdocs && pwd
dd if=/dev/zero of=4G-upload.dat bs=1G count=4
printf "GET /4G-upload.dat HTTP/1.1\r\nHost: extern\r\n\r\n" | nc localhost 31337 > 4G-downloaded.dat
ls -lah 4G-downloaded.dat | awk '{ print "Size of downloaded file: "  $5}'
rm 4G-downloaded.dat
rm 4G-upload.dat
