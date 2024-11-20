--DO $find_private_client$
DECLARE
find_client_type deal_member_type;
find_client_private_id integer;
res record; 
BEGIN
SELECT
client_data.client_type,
client_data.private_id 
INTO 
find_client_type,
find_client_private_id
FROM
client_account
INNER JOIN
client_data
ON 
client_account.client_id = client_data.id
WHERE
client_account.id = 0000000001;

IF find_client_type = 'natural person' THEN
SELECT
first_name,
middle_name,
last_name,
birth_date,
address,
phone_number
INTO res
FROM
natural_client_private
WHERE
id = find_client_private_id;

--select * from natural_client_private;
--RAISE NOTICE 'Result: %', natural_client_info;

ELSIF find_client_type = 'juridical person' THEN
SELECT
primary_state_registration_number,
organization_name,
organization_form,
foundation_date,
address,
phone_number
INTO res
FROM
juridical_client_private
WHERE
id = find_client_private_id;

--RAISE NOTICE 'Result: %', juridical_client_info;
END IF;

--select * from res;

END;
--$find_private_client$;
