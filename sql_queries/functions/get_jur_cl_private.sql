CREATE FUNCTION Get_Jur_Cl_Private_Data(cl_private_id integer)
RETURNS TABLE(
primary_state_registration_number varchar(13),
organization_name varchar(50),
organization_form organization_form,
foundation_date date,
address address,
phone_number varchar(11))
AS $$
BEGIN
RETURN QUERY
SELECT
jcp.primary_state_registration_number,
jcp.organization_name,
jcp.organization_form,
jcp.foundation_date,
jcp.address,
jcp.phone_number
FROM
juridical_client_private jcp
WHERE
id = cl_private_id;
END;
$$ LANGUAGE plpgsql;