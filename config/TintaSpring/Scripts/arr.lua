--[[
arr = {}


function s_arrlength(T)
  local countOut = 0
  for _ in pairs(T) do countOut = countOut + 1 end
  return countOut
end

arr[1] = {1,12,33}

arr[2] = {21,13,99}


--util.msg(arr[2][3])


arra = {1,3,5}
arrb = {2,4,6}

arra = arrb

table.remove(arra, 1)



function addRule( arrRules, arrExcept) 
	local length = s_arrlength(arrRules)
	arrRules[length + 1] = arrExcept
end 

function formList( arrRules, newArr, rule ) 
	
	for i,v in pairs(arrRules) do 
	
		local except = false
		
		for i_,v_ in pairs(v) do 
			if v_ == rule then
				except = true
				break
			end
		end
		
		if except ~= true then
			table.insert(newArr,i)
		end
		
	end
   
end 


local rules = {}
addRule( rules, {1,4}) 
addRule( rules, {2,1}) 
addRule( rules, {3,2}) 

newList = {}

formList( rules, newList, 2 ) 



for i_,v_ in pairs(newList) do 
	util.msg(v_)		
end
		
--util.msg(rules[3][2])

]]--

t = { 1,2,"three", 3.14159, banana="yellow" }
i = t[4]
i = i + 2.0

util.msg(i)