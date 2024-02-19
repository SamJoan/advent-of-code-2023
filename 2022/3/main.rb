
class Rucksack
  attr_accessor :a, :b

  def initialize(a, b)
    @a = a
    @b = b
  end
end

def parse_rucksacks(filename)
  rucksacks = []
  File.readlines(filename).each do |line|
    rucksack = line.strip
    len = rucksack.length
    a, b = rucksack.chars.each_slice(len / 2).map(&:join)

    rucksacks.push(Rucksack.new(a, b))
  end

  return rucksacks
end

def unique_chars(pocket)
  unique = {}
  pocket.each_char do |c|
    unique[c] = true
  end

  return unique.keys
end

def score(c)
  is_lowercase = !(c === c.capitalize)
  if is_lowercase
    return c.ord - 96
  else
    return c.ord - 64 + 26
  end
end

def solve_part_1(rucksacks)
  result = 0
  rucksacks.each do |r|
    a_chars = unique_chars r.a
    b_chars = unique_chars r.b
    common = a_chars & b_chars
    if common.length != 1
      puts "Invalid length #{common.length}"
      exit 1
    end

    result += score(common[0])
  end

  return result
end

def union(a, b)
  return a & b
end

def solve_part_2(rucksacks)
  result = 0
  group_of_three = []
  rucksacks.each.with_index do |r, i|
    group_of_three.push(unique_chars(r.a + r.b))
    if group_of_three.length == 3
      unique =  group_of_three[0] & group_of_three[1] & group_of_three[2]
      if unique.length != 1
        puts "Invalid len '#{unique}'"
        exit 1
      end

      result += score(unique[0])

      group_of_three = []
    end
  end

  return result
end

r = parse_rucksacks(ARGV[0])

result = solve_part_1(r)
puts "Part 1\n#{result}"
result = solve_part_2(r)
puts "Part 2\n#{result}"
