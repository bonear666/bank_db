CREATE FUNCTION Get_Nat_Cl_Private_Data(cl_private_id integer)
RETURNS TABLE(
first_name varchar(20),
middle_name varchar(20),
last_name varchar(20),
birth_date date,
address address,
phone_number varchar(11))
AS $$
BEGIN
RETURN QUERY
SELECT
ncp.first_name,
ncp.middle_name,
ncp.last_name,
ncp.birth_date,
ncp.address,
ncp.phone_number
FROM
natural_client_private ncp
WHERE
id = cl_private_id;
END;
$$ LANGUAGE plpgsql;