class Directory
  attr_accessor :children, :parent

  def initialize(parent)
    @children = {}
    @parent = parent
  end
end

def parse_tree(filename)
  root = Directory.new(nil)
  cwd = root
  File.readlines(filename).drop(1).each do |line|
    line = line.strip
    splat = line.split(" ")
    p line
    p cwd
    if line.start_with? "$ cd"
      dir = splat[2]
      if dir == ".."
        cwd = cwd.parent
      else
        cwd = cwd.children[dir]
        if !cwd
          puts "Cannot find dir #{dir}"
          exit 1
        end
      end
    elsif !line.start_with? "$"
      if splat[0] == "dir"
        cwd.children[splat[1]] = Directory.new(nil)
      else
        cwd.children[splat[1]] = splat[0]
      end
    end
  end
end

tree = parse_tree(ARGV[0])
