--[[
	Задача на 5
	Дана строка 
	local text = "There was nothing so very remarkable in that; nor did Alice think it so very much out of the way to hear the Rabbit say to itself, ‘Oh dear! Oh dear! I shall be late!’ (when she thought it over afterwards, it occurred to her that she ought to have wondered at this, but at the time it all seemed quite natural); but when the Rabbit actually took a watch out of its waistcoat-pocket, and looked at it, and then hurried on, Alice started to her feet, for it flashed across her mind that she had never before seen a rabbit with either a waistcoat-pocket, or a watch to take out of it, and burning with curiosity, she ran across the field after it, and fortunately was just in time to see it pop down a large rabbit-hole under the hedge."
	Написать программу которая будет перемешивать слова !(не буквы) в этом тексте
	Для создания строки из массива(метатаблицы ) с разделителем " ", использовать  table.concat(имя_таблицы_массива," ") 
]]--


local text = "There was nothing so very remarkable in that; nor did Alice think it so very much out of the way to hear the Rabbit say to itself, ‘Oh dear! Oh dear! I shall be late!’ (when she thought it over afterwards, it occurred to her that she ought to have wondered at this, but at the time it all seemed quite natural); but when the Rabbit actually took a watch out of its waistcoat-pocket, and looked at it, and then hurried on, Alice started to her feet, for it flashed across her mind that she had never before seen a rabbit with either a waistcoat-pocket, or a watch to take out of it, and burning with curiosity, she ran across the field after it, and fortunately was just in time to see it pop down a large rabbit-hole under the hedge."

-- функция получения размера метатаблицы
function arrlength(T)
  local countOut = 0
  for _ in pairs(T) do countOut = countOut + 1 end
  return countOut
end



local len = string.len( text )

local tokens = {}

local beginPos = 0
for i = 1, len do

 --  util.msg( string.sub(text, i, i) ) 
	local char = string.sub(text, i, i)
	if char == " " or i == len then		
	
		if beginPos < i then			
			local word = string.sub( text, beginPos, i )
			-- сохраняем отдельные слова в массив
			table.insert( tokens, word )							
			beginPos = i				
		end
		
	end
end

-- перемешиваем слова в массиве число итераций берем из количества слов
local count = arrlength(tokens)
for i = 1, count do
	local from = math.random(1,count)
	local to = math.random(1,count)
	
	local saveTo = tokens[to]
	tokens[to] = tokens[from]
	tokens[from] = saveTo	
end

local rezString = ""

local rezString = table.concat(tokens, " ")-- .. "\n"
util.msg( rezString )
--for i,v in ipairs(tokens) do  
	--util.msg( v )
--end

