DO $find_private_client$
DECLARE
find_client_type deal_member_type;
find_client_private_id integer;
BEGIN
SELECT
client_data.client_type INTO find_client_type,
client_data.private_id INTO find_client_private_id
FROM
client_account
INNER JOIN
client_data
ON 
client_account.client_id = cleint_data.id
WHERE
client_account.id = 0000000000;

CASE
WHEN
find_client_type = 'natural person' 
THEN
(SELECT
first_name,
middle_name,
last_name,
birth_date,
address,
phone_number
FROM
natural_client_private
WHERE
id = find_client_private_id)

WHEN
find_client_type = 'juridical person'
THEN
(SELECT
primary_state_registration_number,
organization_name,
organization_form,
foundation_date,
address,
phone_number
FROM
juridical_client_private
WHERE
id = find_client_private_id)
END;

END;
$find_private_client$;