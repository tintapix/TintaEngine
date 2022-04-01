



local points = { pointsT }


function addPoint( id, pid, x,y )

--[[
pointT.ID = id
pointT.pID = pid
pointT.length = 0
pointT.x = x
pointT.y = y
]]--
local pointT = { ID = id, pID = pid, Length = 0, X = x, Y = y }
table.insert( points, pointT )

end

addPoint( 1, -1, 45,94 )
addPoint( 2, -1, 45,86 )
addPoint( 3, -1, 50,79 )
addPoint( 4, -1, 55,61 )
addPoint( 5, -1, 59,67 )
addPoint( 6, -1, 53,65 )
addPoint( 7, -1, 47,56 )
addPoint( 8, 5,  64,65 )
addPoint( 9, -1, 70,59 )


prev = {}
for k,v in pairs(points) do  
	
	if v.pID ~= -1 then
		v.Length = points[v.pID].Length + math.sqrt( ( points[v.pID].X - v.X)*(points[v.pID].X - v.X) + (points[v.pID].Y - v.Y)*(points[v.pID].Y - v.Y) ) 	
		util.msg( v.Length )
	else
		if prev.X ~= nil then
			--util.msg( prev.X, " ", v.X )
			v.Length = prev.Length + math.sqrt( (prev.X - v.X)*(prev.X - v.X) + (prev.Y - v.Y)*(prev.Y - v.Y) ) 	
		end
	end
	
	prev = v
	
end


local imgPath = "c:/foto/graph.png"
local imgPathOut = "c:/foto/graph_out.png"

local picImg = image.read( imgPath ,"png" )
	

local w = image.width()
local h = image.height()	

local maxLen = 0

for h_ = 0, h-1 do	
			for w_ = 0, w-1 do							
			
				local r,g,b,a = image.getpixelb( w_, h_ )
				
				if a > 0 then		
				
					local minLen = 99999999
					local nearId = -1
					
					for k,v in pairs(points) do  
					
						l = math.sqrt( (w_ - v.X)*(w_ - v.X) + (h_ - v.Y)*(h_ - v.Y) ) 	
						
						if l < minLen then
							minLen = l
							nearId = v.ID
						end
					end
					
					
					if (minLen + points[nearId].Length) > maxLen then
						maxLen = minLen + points[nearId].Length
					end
					--util.msg((minLen + points[nearId].Length)/maxLen )
					--image.setpixelf( w_, h_, ( minLen + points[nearId].Length )/maxLen, 0,0, 1 )
					
				end							
			
			end		
end


for h_ = 0, h-1 do	
			for w_ = 0, w-1 do							
			
				local r,g,b,a = image.getpixelb( w_, h_ )
				
				if a > 0 then		
				
					local minLen = maxLen
					local nearId = -1
					
					for k,v in pairs(points) do  
					
						l = math.sqrt( (w_ - v.X)*(w_ - v.X) + (h_ - v.Y)*(h_ - v.Y) ) 	
						
						if l < minLen then
							minLen = l
							nearId = v.ID
						end
					end
					--util.msg((minLen + points[nearId].Length)/maxLen )
					image.setpixelf( w_, h_, ( minLen + points[nearId].Length )/maxLen, 0,0, 1 )
					
				end							
			
			end		
end

image.save( "png", imgPathOut )

image.erase( picImg )

