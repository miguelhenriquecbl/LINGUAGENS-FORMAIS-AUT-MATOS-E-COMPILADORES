{ Teste 004 - Programa correto testando case-insensitive }
PROGRAM CaseInsensitive;
VAR
    Contador : INTEGER;
    Media    : REAL;
BEGIN
    Contador := 0;
    Media := 0.0;
    WHILE Contador <= 10 DO
    BEGIN
        Contador := Contador + 1;
        Media := Media + 1.5
    END;
    IF Media > 5 THEN
        Contador := Contador - 1
    ELSE
        Contador := Contador + 1
END.
