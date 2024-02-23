
def solve_part_1(radio)
  sop_len = 4
  (0..radio.length-sop_len).each do |s|
    msg = radio[s, sop_len]
    if msg.chars.uniq.length == sop_len
      return s + sop_len
    end
  end 

  puts "Cannot find start of message"
  exit 1
end

def solve_part_2(radio)
  sop_len = 14
  (0..radio.length-sop_len).each do |s|
    msg = radio[s, sop_len]
    if msg.chars.uniq.length == sop_len
      return s + sop_len
    end
  end 

  puts "Cannot find start of message"
  exit 1
end

first_line = File.open(ARGV[0], &:readline).strip


result = solve_part_1(first_line)
puts "Part 1:\n#{result}"

result = solve_part_2(first_line)
puts "Part 2:\n#{result}"

