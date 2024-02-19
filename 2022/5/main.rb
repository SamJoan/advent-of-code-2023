
class Instruction
  attr_accessor :amount, :from, :to

  def initialize(amount, from, to)
    @amount = amount
    @from = from
    @to = to
  end
end

def parse_stacks(stacks_str)
  stacks = []
  len = 0
  stacks_str.reverse_each.with_index do |line, i|
    if i == 0
      len = line.split.length
      (0..len-1).each do |stack_index|
        stacks[stack_index] = []
      end
    else
      (0..len-1).each do |stack_index|
        pos = (stack_index * 4) + 1
        c = line[pos]
        if c != " " and c != nil
          stacks[stack_index].push(c)
        end
      end
    end
  end

  return stacks
end

def parse_instructions(instructions_str)
  instructions = []
  instructions_str.each do |line|
    splat = line.split()
    amount = splat[1]
    from = splat[3]
    to = splat[5]

    instructions.push(Instruction.new(amount.to_i, from.to_i - 1, to.to_i - 1))
  end

  return instructions
end

def parse_input(filename)
  stacks_str = []
  instructions_str = []
  parsing_instructions = false
  File.readlines(filename).each do |line|
    line = line.rstrip
    if line == ""
      parsing_instructions = true
      next
    end

    if !parsing_instructions
      stacks_str.push(line)
    else
      instructions_str.push(line)
    end
  end
  stacks = parse_stacks(stacks_str)
  instructions = parse_instructions(instructions_str)

  return stacks, instructions
end

def solve_part_1(stacks, instructions)
  instructions.each do |instruction|
    stacks[instruction.from].pop(instruction.amount).reverse_each do |elem|
      stacks[instruction.to].push(elem)
    end
  end

  result = ""
  stacks.each do |arr|
    result += arr.pop
  end

  return result
end

def solve_part_2(stacks, instructions)
  instructions.each do |instruction|
    stacks[instruction.from].pop(instruction.amount).each do |elem|
      stacks[instruction.to].push(elem)
    end
  end

  result = ""
  stacks.each do |arr|
    result += arr.pop
  end

  return result
end

stacks, instructions = parse_input(ARGV[0])
result = solve_part_1(stacks, instructions)
puts "Part 1\n#{result}"

stacks, instructions = parse_input(ARGV[0])
result = solve_part_2(stacks, instructions)
puts "Part 2\n#{result}"
