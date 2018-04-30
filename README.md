# mjsonlib
Small JSON library based on M*LIB.

This library allows parsing a JSON file, reading values from it
and output it.

## USAGE

### mjson_t

The type represening a JSON structure.

### void json\_init (mjson\_t *p);

Initialize the JSON structure pointed by p to an empty JSON.

### bool json\_init\_in\_str(mjson\_t *p, FILE *f);

Initialize the JSON structure pointed by p to the contain of the file f
which is supposed to be a JSON file.

### bool json\_init\_set(mjson\_t *p, mjson\_t o);

Initialize the JSON structure pointed by p to the same value than 'o'.

### bool json\_set(mjson\_t *p, mjson\_t o);

Set the JSON structure pointed by p to the same value than 'o'.

### void json\_clear (mjson\_t p);

Clear the JSON structure 'p'.

### bool json\_in\_str(mjson\_t *p, FILE *f);

Set the JSON structure pointed by p to the contain of the file f
which is supposed to be a JSON file.

### void json\_out\_str(FILE *f, mjson\_t p);

Output the JSON structure 'p' into the FILE f.

### double json\_get\_real(mjson\_t p, ...);

Return the real value associated to the given address of the JSON structure 'p', or NAN if no real value is associated to this address.

The address is composed of a NULL terminated list of additional
parameters of type 'const char *'. Each parameter is either the field
associated to the structure or an index in a table (if it starts with
'[').

Example:
           double d = json_get_real(p, "c", "[2]", "d", NULL);
returns the value of p.c.[2].d

### int json\_get\_boolean(mjson\_t p, ...);

Return the boolean value associated to the given address of the JSON structure 'p' (false or true), or -1 if no boolean value is associated to this address.

### const char *json\_get\_string(mjson\_t p, ...);

Return a pointer to a constant string associated to the given address of the JSON structure 'p' (false or true), or -1 if no string value is associated to this address.

### bool json_get_empty(mjson_t, ...);

Return true if the given address is associated to a null value,
or false if the given address doesn't exist or is not associated to a null
value.

## Example

See test.c
