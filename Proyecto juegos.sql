CREATE TABLE Clientes (
    id INT AUTO_INCREMENT PRIMARY   KEY,
    nombre VARCHAR(100),
    telefono VARCHAR(20) 
);

CREATE TABLE Juegos (
    id INT AUTO_INCREMENT PRIMARY   KEY,
    nombre VARCHAR(100),
    precio DECIMAL(10, 2) 
);

CREATE TABLE Reservas (
    id INT AUTO_INCREMENT PRIMARY KEY,
    cliente_id INT,
    fecha_hora TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    total DECIMAL(10, 2),
    FOREIGN KEY (cliente_id) REFERENCES Clientes(id)
);

CREATE TABLE Detalles_Reservas (
    id INT AUTO_INCREMENT PRIMARY KEY,
    reserva_id INT,
    juego_id INT,
    cantidad INT,
    subtotal DECIMAL(10, 2),
    FOREIGN KEY (reserva_id) REFERENCES Reservas(id),
    FOREIGN KEY (juego_id) REFERENCES Juegos(id)
);

INSERT INTO Clientes (nombre, telefono) VALUES
('Juan Pérez', '123456789'),
('Luis García', '987654321'),
('Pedro López', '555123456'),
('Diego Martínez', '666789456'),
('Carlos Rodríguez', '999888777');

INSERT INTO Juegos (nombre, precio) VALUES
('FIFA 22', 400.00),
('Red Dead Redemption 2', 350.00),
('The Legend of Zelda: Breath of the Wild', 400.00),
('Cyberpunk 2077', 350.00),
('Assassins Creed Valhalla', 350.00);

INSERT INTO Reservas (cliente_id, total) VALUES
(1, 400.00),
(2, 350.00),
(3, 400.00),
(4, 350.00),
(5, 350.00);

INSERT INTO Detalles_Reservas (reserva_id, juego_id, cantidad, subtotal) VALUES
(1, 1, 1, 400.00),
(1, 3, 1, 400.00),
(2, 2, 1, 350.00),
(3, 3, 1, 400.00),
(4, 4, 1, 350.00),
(4, 5, 1, 350.00),
(5, 1, 1, 400.00),
(5, 2, 1, 350.00);

SELECT * FROM Clientes

SELECT * FROM Juegos

SELECT * FROM Reservas

SELECT * FROM Detalles_Reservas