--информация о юридических лицах
create table juridical_client_private(
id serial primary key,
primary_state_registration_number varchar(13),
organization_name varchar(50),
organization_form organization_form,
foundation_date date,
address address,
phone_number varchar(11));

--личная информация клента
create table natural_client_private(
id serial primary key,
first_name varchar(20), --имя
middle_name varchar(20), --отчество
last_name varchar(20), --фамилия
birth_date date, --дата рождения
address address, --адрес проживания
phone_number varchar(11)); --номер телефона

--общая таблица всех транзакций всех клиентов
create table transaction_data(
id serial primary key,
client_transaction transaction_token);

--информация о клиентах банка
create table client_data(
id serial primary key,
client_type deal_member_type,
private_id integer, --личная информация, указывает на значение в таблице juridical_client_private или в таблице natural_client_private
account_id integer,
transaction_list_id integer,
foreign key(transaction_list_id) references transaction_data(id) --информация о транзакциях
on delete set null on update cascade,
debit_id integer[], --все дебетовые счета
credit_id integer[], --все кредитные счета
deposit_id integer[], -- все вклады
card_id integer[]); --все карты

--информация учетной записи клиента
create table client_account(
id serial primary key,
client_login varchar(25), --логин для входа
client_password varchar(25), --пароль для входа
client_id integer,
foreign key(client_id) references client_data(id) --ссылка на данные клиента
on delete set null on update cascade);

--информация о дебетовых счетах клиентов
create table debit_data(
id serial primary key,
transfer_number transfer_number, --расчетный счет
debit_type_id integer,
foreign key(debit_type_id) references debit_type(id)
on delete set null on update cascade,
currency_name currency_name,
balance float);

--информация о картах клиентов
create table card_data(
id serial primary key,
card_number varchar(20),
production_date date, --дата выпуска
card_validity date, --срок действия
cvv varchar(3), --cvv код
pin_code varchar(4),
card_type card_type,
--credit_id integer,
--foreign key(credit_id) references credit_data(id) --привязка карты к определенному счет, к одному из двух(credit, debit)
--on delete set null on update cascade,
debit_id integer,
foreign key(debit_id) references debit_data(id)
on delete set null on update cascade,
payment_system_id integer,
foreign key(payment_system_id) references card_payment_system_type(id)
on delete set null on update cascade);

--информация о вкладах клинетов
create table deposit_data(
id serial primary key,
transfer_number transfer_number,
deposit_type_id integer,
foreign key(deposit_type_id) references deposit_type(id)
on delete set null on update cascade,
balance float);

--информация о кредитных счетах клиентов
--create table credit_data(
--id serial primary key,
--transfer_number transfer_number, --расчетный счет
--foreign key(credit_type_id) references credit_type(id)
--on delete set null on update cascade,
--currency_name currency_name,
--balance float,
--repayment float); --остаток для погашения

--информация о транзакциях клиента
create table client_transaction_data(
id serial primary key,
client_id integer,
foreign key(client_id) references client_data(id)
on delete set null on update cascade,
client_transaction_list integer[]); --список всех транзакций, в которых участвовал клиент. Массив id транзакций из таблицы transaction_data.

--кредитная история клиента ???


