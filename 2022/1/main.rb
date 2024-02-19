def parse_calories(filename)
  calories = {}
  number = 0;
  File.readlines(filename).each do |line|
    if line.strip == ""
      number += 1
    else
      if !calories.key? number
        calories[number] = []
      end

      calories[number].push(line.to_i)

    end
  end

  return calories
end

def solve_part_1(calories)
  max = 0
  calories.each do |key, val|
    elf_total = 0
    val.each do |calories|
      elf_total += calories
    end

    max = elf_total > max ? elf_total : max
  end

  return max
end

def solve_part_2(calories)
  values = []
  calories.each do |key, val|
    elf_total = 0
    val.each do |calories|
      elf_total += calories
    end

    values.push(elf_total)
  end

  top_three = values.sort.reverse.first(3)
  return top_three.reduce(0, :+)
end

c = parse_calories(ARGV[0])

result = solve_part_1(c)
puts "Part 1:\n #{result}"

result = solve_part_2(c)
puts "Part 2:\n #{result}"


