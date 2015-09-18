require 'rubygems'
require 'serialport'

serial = ARGV[0] || "/dev/ttyACM1"

puts "Sending on "+serial
sp = SerialPort.new serial, 9600
puts "Reset"
sp.write "I"
sleep 3
while true
  current = Time.now
  if current.month < 10
    sp.write "#{current.day}0#{current.month}"
  else
    sp.write "#{current.day}#{current.month}"
  end
  sleep 3
  sp.write current.year
  sleep 3
  current = Time.now
  if current.min < 10
    sp.write "#{current.hour}0#{current.min}"
  else
    sp.write "#{current.hour}#{current.min}"
  end
  sleep 10
end
