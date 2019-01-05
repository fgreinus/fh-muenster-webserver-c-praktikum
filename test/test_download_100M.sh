# This shell test was written in a similar form by psef15 @ SS/14

cd "$( dirname "${BASH_SOURCE[0]}" )"/../htdocs && pwd
dd if=/dev/zero of=100M-upload.dat bs=1M count=100
printf "GET /100M-upload.dat HTTP/1.1\r\nHost: extern\r\n\r\n" | nc localhost 31337 > 100M-downloaded.dat
ls -lah 100M-downloaded.dat | awk '{ print "Size of downloaded file: "  $5}'
rm 100M-downloaded.dat
rm 100M-upload.dat
