program Completo;
var
    x, y : integer;
    z    : real;
begin
    x := 10;
    y := 20;
    z := 1.5;
    while x <= y do
    begin
        x := x + 1;
        z := z * 2.0
    end;
    if z > 10 then
        x := 0
    else
        x := 1
end.
