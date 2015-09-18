require 'rubygems'
require 'serialport'

serial = ARGV[0] || "/dev/ttyACM1"

puts "Sending on "+serial
sp = SerialPort.new serial, 9600
puts "Reset"
sp.write "R"
sleep 3
sp.write "-1"
sleep 1
current = Time.now
value = current.hour * 3600+ current.min*60+current.sec
puts "Sending #{current.hour}:#{current.min}:#{current.sec.to_s} (#{value})"
sp.write value.to_s
