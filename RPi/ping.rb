require 'rubygems'
require 'serialport'
require 'net/ping'

serial = ARGV[0] || "/dev/ttyACM1"

puts "Sending on "+serial
sp = SerialPort.new serial, 9600

puts "Reset"
sp.write "I"
sleep 3
while true do
  ping = Net::Ping::HTTP.new("http://google.com")
  if ping.ping?
   data = (ping.duration * 1000).to_i
   puts "Ping to google.com : #{data}ms"
   sp.write data.to_s
  end
  sleep(1)
end
