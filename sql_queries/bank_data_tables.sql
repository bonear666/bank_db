--информация о филиалах банка
create table bank_office_data(
id serial primary key,
address address);

--личная информация сотрудника
create table employee_private(
id serial primary key,
first_name varchar(20), --имя
middle_name varchar(20), --отчество
last_name varchar(20), --фамилия
birth_date date, --дата рождения
address address, --адрес проживания
phone_number varchar(11)); --номер телефона

--личный кабинет сотрудников
create table employee_account(
id serial primary key,
client_login varchar(25), --логин для входа
client_password(25)); --пароль для входа

--информация об отделах в банках
create table departament_data(
id serial primary key,
deparament_name varchar(50),
employee_amount integer,
foreign key(bank_office_id) references bank_office_data(id) 
on delete set null on update cascade);

--информация о сотрудниках банка
create table bank_employee_data(
id serial primary key,
job_type job_type,
foreign key(private_id) references employee_account(id) 
on delete set null on update cascade,
foreign key(account_id) references departament_data(id) 
on delete set null on update cascade,
foreign key(departament_id) references departament_data(id) 
on delete set null on update cascade);
