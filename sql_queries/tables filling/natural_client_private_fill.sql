INSERT INTO natural_client_private
(first_name, middle_name, last_name, birth_date, address, phone_number)
VALUES
('Alberto', 'Petrovich', 'Bocharov', '1990-01-17', ROW('Russia', 'Ekaterinburg', 'Pushkina', 12)::address, '12345'),
('Vin', null, 'Diesel', '1970-09-21', ROW('USA', 'New-York', 'Lenin street', 40)::address, '1337'),
('Valeriy', 'Albertovich', 'Zhmishenko', '1952-11-01', ROW('Russia', 'Samara', 'Enot street', 54)::address, '12345'),
('Dwayne', 'Rock', 'Johnson', '1972-09-02', ROW('USA', 'Los-Angeles', 'Groove street', 4)::address, '12345'),
('Lana', 'Del', 'Rey', '1985-06-21', ROW('Russia', 'Chelyabinsk', 'Pushkina', 123)::address, '12345');