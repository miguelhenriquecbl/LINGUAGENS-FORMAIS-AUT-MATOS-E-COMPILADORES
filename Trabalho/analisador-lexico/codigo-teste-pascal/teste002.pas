{ Teste 002 - Programa correto focado em operadores relacionais e expressoes }
program Operadores;
var
    a, b, c : integer;
begin
    a := 100;
    b := 200;
    c := 0;
    c := a + b;
    c := a - b;
    c := a * b;
    c := b / a;
    if a <> b then
        c := a + 1;
    if a < b then
        c := b - 1;
    if a <= b then
        c := c + 1;
    if b > a then
        c := c - 1;
    if b >= a then
        c := c + 10;
    if a = c then
        c := 0
end.
