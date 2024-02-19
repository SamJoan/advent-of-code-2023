class Hand
  attr_accessor :hand_a, :hand_b, :expected_result

  def initialize(hand_a, hand_b)
    @hand_a = symbol(hand_a)
    @hand_b = symbol(decrypt(hand_b))
    @expected_result = symbol_result(hand_b)
  end

  def decrypt(letter)
    if letter == "X"
      return "A"
    elsif letter == "Y"
      return "B"
    elsif letter == "Z"
      return "C"
    end

    puts "Invalid letter for encrypted card"
    exit 1
  end

  def symbol_result(letter)
    if letter == "X"
      return :lose
    elsif letter == "Y"
      return :draw
    elsif letter == "Z"
      return :win
    else
      puts "Invalid letter #{letter}"
      exit 1
    end
  end

  def symbol(letter)
    if letter == "A"
      return :rock
    elsif letter == "B"
      return :paper
    elsif letter == "C"
      return :scissors
    else
      puts "Invalid letter #{letter}"
      exit 1
    end
  end
end

def parse_hands(filename)
  hands = []
  File.readlines(filename).each do |line|
    splat = line.strip.split(" ")
    hands.push(Hand.new(splat[0], splat[1]))
  end

  return hands
end

def play_hand(you, me)
  if you == :rock
    if me == :rock
      return :draw
    elsif me == :paper
      return :win
    elsif me == :scissors
      return :lose
    end
  elsif you == :paper
    if me == :paper
      return :draw
    elsif me == :scissors
      return :win
    elsif me == :rock
      return :lose
    end
  elsif you == :scissors
    if me == :scissors
      return :draw
    elsif me == :rock
      return :win
    elsif me == :paper
      return :lose
    end
  end

  puts "invalid hand"
  exit 1
end

def solve_part_1(hands)
  total_score = 0
  hands.each do |hand|
    a = hand.hand_a
    b = hand.hand_b

    result = play_hand(a, b) 

    round_score = 0
    if b == :rock
      round_score += 1
    elsif b == :paper
      round_score += 2
    elsif b == :scissors
      round_score += 3
    end
    
    if result == :draw
      round_score += 3
    elsif result == :win
      round_score += 6
    end

    total_score += round_score
  end

  return total_score
end

def determine_card_needed(you, expected_result)
  if you == :rock
    if expected_result == :draw
      return :rock
    elsif expected_result == :win
      return :paper
    elsif expected_result == :lose
      return :scissors
    end
  elsif you == :paper
    if expected_result == :draw
      return :paper
    elsif expected_result == :win
      return :scissors
    elsif expected_result == :lose
      return :rock
    end
  elsif you == :scissors
    if expected_result == :draw
      return :scissors
    elsif expected_result == :win
      return :rock
    elsif expected_result == :lose
      return :paper
    end
  end

  puts "Invalid value for #{you}"
  exit 1
end

def solve_part_2(hands)
  total_score = 0
  hands.each do |hand|
    a = hand.hand_a
    expected_result = hand.expected_result
    b = determine_card_needed(a, expected_result)

    result = play_hand(a, b) 

    if result != expected_result
      puts "Invalid result #{result}, expected #{expected_result}"
      exit 1
    end

    round_score = 0
    if b == :rock
      round_score += 1
    elsif b == :paper
      round_score += 2
    elsif b == :scissors
      round_score += 3
    end
    
    if result == :draw
      round_score += 3
    elsif result == :win
      round_score += 6
    end

    total_score += round_score
  end

  return total_score
end

hands = parse_hands(ARGV[0])

score = solve_part_1(hands)
puts "Part 1:\n#{score}"

score = solve_part_2(hands)
puts "Part 2:\n#{score}"

