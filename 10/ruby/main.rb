DIRECTIONS = [:up, :down, :left, :right]

CHARS = {
  '|' => [:up, :down],
  '-' => [:left, :right],
  'L' => [:up, :right],
  'J' => [:up, :left],
  '7' => [:down, :left],
  'F' => [:down, :right]
}

def parse_map(filename)
  fh = File.open(filename)
  map = []
  fh.each do |line|
    map.push(line.strip)
  end

  return map
end

class Coords
  attr_accessor :x, :y
  def initialize(map, x, y)
    @x = x
    @y = y
    @map = map
  end

  def up
    if @y - 1 < 0
      return nil
    else 
      return Coords.new(@map, @x, @y - 1)
    end
  end

  def down
    if @y + 1 > @map.length - 1
      return nil
    else
      return Coords.new(@map, @x, @y + 1)
    end
  end

  def left
    if @x - 1 < 0
      return nil
    else
      return Coords.new(@map, @x - 1, @y)
    end
  end

  def right
    if @x + 1 > @map[0].length - 1
      return nil
    else
      return Coords.new(@map, @x + 1, @y)
    end
  end

  def value
    return @map[@y][@x]
  end

end

def guess_c(input_dirs)
  CHARS.each do |char, dirs|
    if dirs.include? input_dirs[0] and dirs.include? input_dirs[1]
      return char
    end
  end

  puts "Could not find the right starting char"
  exit
end

def discover_starting_character(map, x, y)
  c = Coords.new(map, x, y)

  dirs = []
  if CHARS[c.up.value]&.include? :down
    dirs.push(:up)
  end
  
  if CHARS[c.down.value]&.include? :up
    dirs.push(:down)
  end

  if CHARS[c.left.value]&.include? :right
    dirs.push(:left)
  end

  if CHARS[c.right.value]&.include? :left
    dirs.push(:right)
  end

  if dirs.length == 2
    return guess_c(dirs)
  else
    puts "Failed to guess starting character #{dirs}"
    exit
  end

end

def discover_s(map)
  y = 0
  map.each do |line|
    start_location = line.index('S')
    if start_location
      return start_location, y, discover_starting_character(map, start_location, y)
    end

    y += 1
  end
end

def opposite(dir)
  if dir == :down
    return :up
  elsif dir == :up
    return :down
  elsif dir == :left
    return :right
  elsif dir == :right
    return :left
  else
    puts "Invalid dir"
    exit 1
  end
end

def navigate_map(map, start_x, start_y)
  cur = Coords.new(map, start_x, start_y)
  cur_dir = CHARS[cur.value][0]
  coords = []

  steps = 0
  while steps == 0 || !(cur.x == start_x && cur.y == start_y) do
    coords.push(cur)

    next_dirs = CHARS[cur.value]
    next_dir = opposite(cur_dir) == next_dirs[0] ? next_dirs[1] : next_dirs[0]

    next_coord = cur.send next_dir
    
    cur_dir = next_dir
    cur = next_coord

    steps += 1
  end

  return steps, coords
end

def solve_part_1(filename)
  map = parse_map(filename)
  start_x, start_y, start_c = discover_s map
  map[start_y][start_x] = start_c

  steps, coords = navigate_map(map, start_x, start_y)

  return steps / 2
end

def simplify_map(map, coords)
  new_map = []
  y = 0
  map.each do |line|
    x = 0
    new_map[y] = ""
    line.each_char do |c|
      in_coords = false
      coords.each do |coord|
        if coord.x == x && coord.y == y
          in_coords = true
          break
        end
      end

      if in_coords
        new_map[y] += map[y][x]
      else
        new_map[y] += ' '
      end

      x += 1
    end

    y += 1
  end

  return new_map
end

def calculate_area(map)
  total_area = 0
  map.each do |line|
    inside = false
    line.each_char do |c|
      if c != ' '
        dirs = CHARS[c]
        if dirs.include?(:up)
          inside = !inside
        end
      elsif inside
        total_area += 1
      end
    end
  end

  return total_area
end


def solve_part_2(filename)
  map = parse_map(filename)
  start_x, start_y, start_c = discover_s map
  map[start_y][start_x] = start_c

  steps, coords = navigate_map(map, start_x, start_y)
  map = simplify_map(map, coords)

  area = calculate_area(map)

  return area
end


part1 = solve_part_1(ARGV[0])
puts "Part 1: #{part1}"

part2 = solve_part_2(ARGV[0])
puts "Part 2: #{part2}"
