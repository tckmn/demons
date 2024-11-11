#!/usr/bin/env ruby

def sim demons, towns
    loop {
        rand(demons+towns) < demons ? (demons-=1) : (towns-=1)
        return true if demons==0
        towns -= demons
        return false if towns<=demons
    }
end

# p Array.new(1000){sim 10, 634701}.count true # 498

def search size, demons, guess
    puts "=== search #{demons} ==="
    extras = 10
    tries = -1
    seenlo = false
    seenhi = false
    while (tries -= 1) != 0
        test = Array.new(size){sim demons, guess}.count true
        if test > size/2
            puts "#{guess} hi -> #{test}"
            guess -= 2
            tries = extras if seenlo && !seenhi
            seenhi = true
        else
            puts "#{guess} lo -> #{test}"
            guess += 2
            tries = extras if !seenlo && seenhi
            seenlo = true
        end
    end
    puts
end

#search 100000, 1, 6

#search 100000, 2, 40

#search 50000, 3, 164

#search 50000, 4, 600

$probs = {}

# **nokill
# $shouldkill = {}
# $nkm = {}
# $log = ''

def prob demons, towns
    return 1 if demons == 0
    return 0 if towns <= demons
    return $probs[[demons,towns]] if $probs[[demons,towns]]

    # demon tech, town tech
    yeskill =
        (demons.to_f / (demons+towns)) * ((1...demons).map{|d| prob(demons-1, towns-d) }.min || 1) +
        (towns.to_f  / (demons+towns)) * (1..demons).map{|d| prob(demons, towns-d-1) }.min
    nokill = (1..demons).map{|d| prob(demons, towns-d) }.min

    # only demon tech, no town tech
    # yeskill =
    #     (demons.to_f / (demons+towns)) * ((1...demons).map{|d| prob(demons-1, towns-d) }.min || 1) +
    #     (towns.to_f  / (demons+towns)) * (1..demons).map{|d| prob(demons, towns-d-1) }.min
    # nokill = yeskill

    # only town tech, no demon tech
    # yeskill =
    #     (demons.to_f / (demons+towns)) * prob(demons-1, towns-demons+1) +
    #     (towns.to_f  / (demons+towns)) * prob(demons, towns-demons-1)
    # nokill = prob(demons, towns-demons)

    # no tech
    # yeskill =
    #     (demons.to_f / (demons+towns)) * prob(demons-1, towns-demons+1) +
    #     (towns.to_f  / (demons+towns)) * prob(demons, towns-demons-1)
    # nokill = yeskill

    # **nokill
    # if !($shouldkill[[demons,towns]] = yeskill < nokill ? false : true) && !$nkm[[demons, towns % (demons+1)]]
    #     $nkm[[demons, towns % (demons+1)]] = true
    #     $log += "nokill #{demons} #{towns} (#{towns % (demons+1)} mod #{demons+1})\n"
    #     # puts $log
    # end

    $probs[[demons,towns]] = yeskill < nokill ? nokill : yeskill
end

def dstrat demons, towns
    (1..demons).min_by{|d| prob(demons, towns-d) }
end

# $probs = {}
# $shouldkill = {}
# def prob demons, towns
#     return 1 if demons == 0
#     return 0 if towns <= demons
#     $probs[[demons,towns]] ||= demons.to_f / (demons+towns) * prob(demons-1, towns-demons+1) + towns.to_f / (demons+towns) * prob(demons, towns-demons-1)
#     nokill = prob(demons, towns-demons)
# end

def psearch demons, towns
    puts "=== search #{demons} ==="
    seenlo = false
    seenhi = false
    loop {
        pr = prob demons, towns
        if pr > 0.5
            puts "#{towns} hi -> #{pr}"
            towns -= 1
            break if seenlo && !seenhi
            seenhi = true
        else
            puts "#{towns} lo -> #{pr}"
            towns += 1
            break if !seenlo && seenhi
            seenlo = true
        end
    }
    ans = seenhi ? towns-1 : towns+1
    puts "answer #{ans-1}~#{ans+1}"
    puts
end

def pwalk demons, towns
    # puts "=== walk #{demons} ==="
    while (pr = prob demons, towns) < 0.5
        # puts "#{towns} -> #{pr}"
        # puts "#{towns} -> #{pr}" if towns % 10000 == 0
        towns += 1
    end
    # puts
    towns
end

def describe arr
    diffs = arr.each_cons(2).map{|x,y|y-x}.reverse
    len = (1..diffs.size/3).find{|len|
        slices = diffs.each_slice len
        slices.take(slices.size/2).uniq.size == 1
    }
    if len
        mod = diffs.take(len).sum
        # idx = -1
        # npattern = diffs.take_while{|diff| diff == diffs[(idx+=1)%len]}.size
        upcoming = arr.reverse.take(len).map{|i| [i%mod, i]}.to_h
        p arr.reverse.select{|i|
            if upcoming[i%mod] == i
                upcoming[i%mod] -= mod
                false
            else
                true
            end
        }.reverse
        upcoming.sort_by{|k,v|v}.each do |k,v|
            puts "#{k} mod #{mod}, >= #{v+mod}"
        end
    else
        p arr
    end
end

def describe2 arr, mod
    intervals = Hash.new{|h,k| h[k] = []}
    arr.each do |i|
        m = i%mod
        if intervals[m][-1] && intervals[m][-1][1] == i-mod
            intervals[m][-1][1] = i
        else
            intervals[m].push [i,i]
        end
    end
    p intervals
end

dn = 4
nums = Hash.new{|h,k| h[k] = []}
(dn*2..600000).each do |tn|
    puts tn if tn%10000==0
    dst = dstrat dn, tn
    nums[dst].push tn unless dst == dn
end
nums.sort.each do |dst, tns|
    print "kill #{dst}: "
    describe2 tns, 20
end
exit

dn = 2
nums = Hash.new{|h,k| h[k] = []}
(dn*2..600000).each do |tn|
    dst = dstrat dn, tn
    nums[dst].push tn unless dst == dn
end
nums.sort.each do |dst, tns|
    print "kill #{dst}: "
    describe tns
end
exit

dn = 3
nums = Hash.new{|h,k| h[k] = []}
(dn*2..600000).each do |tn|
    dst = dstrat dn, tn
    nums[dst].push tn unless dst == dn
end
nums.sort.each do |dst, tns|
    print "kill #{dst}: "
    describe2 tns, 12
end
exit

dn = 3
nums = Hash.new{|h,k| h[k] = []}
(dn*2..10000).each do |tn|
    dst = dstrat dn, tn
    nums[dst].push tn unless dst == dn
end
nums.sort.each do |dst, tns|
    print "kill #{dst}: "
    describe tns
end
exit

dn = 3
(dn*2..1000).each do |tn|
    dst = dstrat dn, tn
    puts "#{tn} -> #{dst} #{tn % 12}" unless dst == dn
end
exit

(1..10).each do |dn|
    puts "#{dn} -> #{pwalk dn, 1}"
end
exit

dn = 8
pw = pwalk dn, 1
p (3..pw).select{|x| $shouldkill[[dn,x]]==false}
puts $log
exit

(1..20).each do |tn|
    (1..4).each do |dn|
        print "%.015f " % prob(dn, tn)
    end
    puts
end
exit

dn = 8
pw = pwalk dn, 1
p (3..pw).select{|x| $shouldkill[[dn,x]]==false}
puts $log
exit

puts prob(2, 5)
puts prob(2, 3)
exit

psearch 2, 1
p (3..40).select{|x| $shouldkill[[2,x]]==false}
exit

psearch 11, 1905634
exit

nums = [
6,
39,
166,
603,
2074,
6823,
21780,
68047,
209154,
634631
]
p nums.each_cons(2).map{|x,y|y/x.to_f}
exit

psearch 1, 6
psearch 2, 41
psearch 3, 166
psearch 4, 605
psearch 5, 2074
psearch 6, 6823
psearch 7, 21780
psearch 8, 68049
psearch 9, 209154
psearch 10, 634633
exit


# 4-6
# 37-39
# 164-166
# 601-603
# 2072-2074
# 6821-6823
# 21778-21780
# 68045-68047
# 209152-209154
# 634629-634631

