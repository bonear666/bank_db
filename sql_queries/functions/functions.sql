create function check_transaction(p_table regclass) returns trigger as 
$body$
declare
rows record;
begin
for rows in (select * p_table)
loop
if exists (select transaction_id from unnest(client_transaction_data.client_transaction_list) as transaction_id where transaction_id = old.id)
then 

end if;
end loop;
end;
$body$
language plpsgl;