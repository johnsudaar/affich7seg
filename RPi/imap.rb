require 'rubygems'
require 'serialport'
require 'mail'
require 'openssl'

puts "Password required: "

system "stty -echo"
userPassword = STDIN.gets.chomp
system "stty echo"

Mail.defaults do
  retriever_method :imap, :address    => "jify.net", 
	  :port       => 993, 
	  :user_name  => 'test@jify.net', 
	  :password   => userPassword,
	  :enable_ssl => true 
end


serial = ARGV[0] || "/dev/ttyACM1"

puts "Sending on "+serial
sp = SerialPort.new serial, 9600

puts "Reset"
sp.write "R"
sleep 3
old = 0
sp.write old
while true do
  count = 0
  Mail.find(keys: ['NOT', 'SEEN']) do | email, imap, uid |
    count = count +1 
    imap.uid_store(uid, "-FLAGS", [:Seen])
  end
  if count !=  old 
    sp.write count
    puts count
    old = count
  end
  sleep(2)
end
