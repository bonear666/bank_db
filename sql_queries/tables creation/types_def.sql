create type address as(country varchar(25), city varchar(25), street varchar(25), num int);

create type currency_name as enum('ruble', 'dollar', 'euro');

create type currency_data as(currency_name currency_name, currency_value float);

create type card_type as enum('credit', 'debit');

create type deal_member_type as enum('natural person', 'juridical person');

create type organization_form as enum();

create type transfer_number as(transfer_number varchar(20));

create type transaction_token as(
sender transfer_number, 
recipient transfer_number, 
sender_type deal_member_type, 
recipient_type deal_member_type,
transfer_value float, 
currency_name currency_name);

create type job_type as enum('distance', 'office');

create type payment_system as enum('visa', 'mir', 'mastercard');