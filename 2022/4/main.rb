
class Interval
  attr_accessor :s, :e

  def initialize(s, e)
    @s = s.to_i
    @e = e.to_i
  end

  def to_s
    return "#{@s},#{@e}"
  end
end

def parse_intervals(filename)
  interval_pairs = []
  File.readlines(filename).each do |line|
    pair = []
    line.strip.split(',').each do |interval_str|
      s, e = interval_str.split('-')
      pair.push(Interval.new(s, e))
    end

    interval_pairs.push(pair)
  end

  return interval_pairs
end

def contains(interval_a, interval_b)
  interval_a.s >= interval_b.s && interval_a.e <= interval_b.e
end

def overlaps(interval_a, interval_b)
  !(interval_a.e < interval_b.s || interval_a.s > interval_b.e)
end

def solve_part_1(interval_pairs)
  contained = 0
  interval_pairs.each do |pair|
    if contains(pair[0], pair[1]) || contains(pair[1], pair[0])
      contained += 1
    end
  end

  return contained
end

def solve_part_2(interval_pairs)
  overlap = 0
  interval_pairs.each do |pair|
    if overlaps(pair[0], pair[1])
      overlap += 1
    end
  end

  return overlap
end



interval_pairs = parse_intervals(ARGV[0])
result = solve_part_1(interval_pairs)
puts "Part 1:\n#{result}"

result = solve_part_2(interval_pairs)
puts "Part 2:\n#{result}"
