-- implements gates and travel between them
-- used in HSE and Eressea

function gate_travel(b, units)
  -- we've found which units we want to exchange, now swap them:
  local u
  for key, u in pairs(units) do
    u.region = b.region
    u.building = b
  end
end

function gate_units(b, maxsize)
  local size = maxsize
  local units = {}
  local u

  for u in b.units do
    if u.number<=size and u.weight<=u.capacity then
      units[u] = u
      size = size - u.number
    end
  end
  return units
end
