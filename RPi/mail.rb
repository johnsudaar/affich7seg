require 'rubygems'
require 'serialport'
require 'gmail'
serial = ARGV[0] || "/dev/ttyACM1"
username = ARGV[1] || "foo.bar"
puts "Enter the "+username+" gmail password:"

system "stty -echo"
userPassword = STDIN.gets.chomp
system "stty echo"


puts "Sending on "+serial
gmail = Gmail.new(username,userPassword)
sp = SerialPort.new serial, 9600
old = -1
while true do
  nums = gmail.mailbox("INBOX").count(:unread)
  if old != nums
    puts "Mails recu : "+nums.to_s
    sp.write nums.to_s
    old = nums
    sleep(2)
  end
end
