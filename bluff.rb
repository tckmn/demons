#!/usr/bin/env ruby

$probs = Array.new(20){[]}
$nokill = Array.new(20){[]}

def prob demons, towns
    return 1.to_r if demons == 0
    return 0.to_r if towns <= demons
    return $probs[demons][towns] if $probs[demons][towns]

    # demon tech, town tech
    yeskill =
        (demons.to_r / (demons+towns)) * ((1...demons).map{|d| prob(demons-1, towns-d) }.min || 1) +
        (towns.to_r  / (demons+towns)) * (1..demons).map{|d| prob(demons, towns-d-1) }.min
    nokill = (1..demons).map{|d| prob(demons, towns-d) }.min

    $nokill[demons][towns] = nokill <=> yeskill
    $probs[demons][towns] = yeskill < nokill ? nokill : yeskill
end

def tstrat demons, towns
    return 0 if demons == 0 || towns <= demons
    prob demons, towns
    $nokill[demons][towns]
end

def dstrat demons, towns
    (1..demons).min_by{|d| prob(demons, towns-d) }
end


class Polynomial
    attr_accessor :terms
    def initialize terms
        @terms = terms
    end
    def self.const n
        self.new({ 0 => n })
    end
    def multvar name
        Polynomial.new @terms.transform_keys{|k|
            abort 'what' if (k & (1 << name)) != 0
            k | (1 << name)
        }
    end
    def *(n); Polynomial.new @terms.transform_values{|v| v*n }; end
    def +(p); Polynomial.new(@terms.merge(p.terms) {|k,a,b| a+b}); end
    def -(p); Polynomial.new(@terms.merge((p*-1).terms) {|k,a,b| a+b}); end
    def func
        terms = @terms.map{|k,v| [k.to_s(2).reverse.chars.map.with_index{|ch,i| ch == ?1 ? i : nil}.compact, v] }
        ->vals { terms.sum{|k,v| k.map{|i|vals[i]}.reduce(1,:*)*v } }
    end
    def partials arr
        arr.map.with_index{|_,i| Polynomial.new(@terms.filter{|k,v| (k & (1 << i)) != 0}.transform_keys{|k| k ^ (1 << i)}).func }
    end
    def to_s
        @terms.map{|k,v| ([v.inspect]+k.to_s(2).reverse.chars.map.with_index{|ch,i| ch == ?1 ? "v#{i}" : nil}.compact)*?*}.*?+
    end
end


$normal = {}

def solve_town demons, towns, history
    if history[-1] != 1 || dstrat(2, demons+towns-1) == 1
        # known 2 demons
        if tstrat(demons, towns) == 1
            solve_demon(demons, towns, history+[0])
        else
            solve_demon(demons-1, towns, history+[1]) * (demons.to_r / (demons+towns)) +
            solve_demon(demons, towns-1, history+[1]) * (towns.to_r  / (demons+towns))
        end
    else
        # might be 1 or 2 demons
        one = tstrat(1, demons+towns-1)
        two = tstrat(2, demons+towns-2)
        if one * two == -1
            # strats disagree, need to pick
            var = $normal[history] ||= $normal.size
            tmp = solve_demon(demons, towns, history+[0])
            tmp - tmp.multvar(var) +
            (
                solve_demon(demons-1, towns, history+[1]) * (demons.to_r / (demons+towns)) +
                solve_demon(demons, towns-1, history+[1]) * (towns.to_r  / (demons+towns))
            ).multvar(var)
        else
            if one == 1 || two == 1
                solve_demon(demons, towns, history+[0])
            else
                solve_demon(demons-1, towns, history+[1]) * (demons.to_r / (demons+towns)) +
                solve_demon(demons, towns-1, history+[1]) * (towns.to_r  / (demons+towns))
            end
        end
    end
end

def solve_demon demons, towns, history
    if demons == 0
        Polynomial.const 1
    elsif demons == 1
        if towns <= 2
            Polynomial.const 0
        else
            solve_town(demons, towns-1, history+[1])
        end
    else
        if towns <= 4
            Polynomial.const 0
        elsif dstrat(demons, towns) == 1
            solve_town(demons, towns-1, history+[1])
        else
            # might bluff
            var = $normal[history] ||= $normal.size
            tmp = solve_town(demons, towns-1, history+[1])
            tmp - tmp.multvar(var) +
            solve_town(demons, towns-2, history+[2]).multvar(var)
        end
    end
end

puts solve_town(2, 8, [])
p $normal
exit

# some failed attempts

poly = solve_town(2, 10, [])
vals = [rand]*$normal.size
vals = [0.534285306146848, 0.2769297317911239, 0.8925219281751428, 1, 0.603857993424659, 0.7927274476931627, 1, 1]
puts poly
p $normal

func = poly.func
partials = poly.partials vals
evil = $normal.map{|k,v| [k.size%2 == 1, v]}.sort_by(&:last).map(&:first)
mult = 1
mult = 0.01
loop {
    p vals
    p func[vals]
    p partials.map{|f| f[vals] }
    p mult
    puts
    5000.times do
    pvs = partials.map{|f| f[vals] }
    # i = partials.each_index.max_by{|i| vals[i] == ((pvs[i] < 0) == evil[i] ? 1 : 0) ? 0 : pvs[i].abs}
    i = rand partials.size
    vals[i] = (vals[i] + pvs[i] * (evil[i] ? -1 : 1) * mult).clamp(0,1)
    # vals[i] = (vals[i] + (pvs[i] != 0 ? pvs[i] * (evil[i] ? -1 : 1) : 0.1*mult*(vals[i]>0.5 ? -1 : 1)) * mult).clamp(0,1)
    end
    mult *= 0.99
}
