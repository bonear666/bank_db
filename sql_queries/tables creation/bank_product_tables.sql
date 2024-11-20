--виды обслуживания счетов
create table service_type(
id serial primary key,
first_cost float,
first_period integer,
second_cost float,
second_period integer,
buy_comission_percent float,
month_comission float);

--виды дебетовых счетов
create table debit_type(
id serial primary key,
product_name text,
current_currency_name currency_name,
available_currency currency_name[],
service_id integer,
foreign key(service_id) references service_type(id) 
on delete set null on update cascade);

--виды кредитов
--create table credit_type(
--id serial primary key,);

--виды  вкладов
create table deposit_type(
id serial primary key,
min_value float,
min_time float,
adding_funds boolean, --возможно ли снятие со счета
drawing_funds boolean, --возможно ли пополнение счета
available_currency currency_name[],
deposit_percent float);

--поддерживаемые картой платежные системы
create table card_payment_system_type(
id serial primary key,
payment_system payment_system[]);

--доступные к конвертации валюты
create table available_currency_type(
id serial primary key,
available_currency currency_name[]);
