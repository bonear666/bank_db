SELECT
client_data.client_type,
client_data.private_id
FROM
client_account
INNER JOIN
client_data
ON 
client_account.client_id = client_data.id
WHERE
client_account.id = 0000000000;
