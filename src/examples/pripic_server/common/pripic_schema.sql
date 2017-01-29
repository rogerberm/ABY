CREATE TABLE user (
    id INTEGER PRIMARY KEY,
    email TEXT,
    face_representation TEXT,
    pw_hash TEXT
);
CREATE INDEX user_email_idx ON user(email);

CREATE TABLE item (
    id INTEGER PRIMARY KEY,
    user_id INTEGER REFERENCES user,
    created TIMESTAMP
);
CREATE INDEX item_user_id_idx ON item(user_id);

CREATE TABLE attribute (
    id INTEGER PRIMARY KEY,
    filename TEXT,
    description TEXT
);

CREATE TABLE itemattribute (
    id INTEGER PRIMARY KEY,
    item_id INTEGER REFERENCES item,
    attribute_id INTEGER REFERENCES attribute
);
CREATE INDEX itemattribute_item_id_idx ON itemattribute(item_id);
CREATE INDEX itemattribute_attribute_id_idx ON itemattribute(attribute_id);

CREATE TABLE photo (
    id INTEGER PRIMARY KEY,
    item_id INTEGER REFERENCES item,
    thumbnail_filename TEXT,
    fullres_filename TEXT,
    fully_approved BOOLEAN
);
CREATE INDEX photo_item_id_idx ON photo(item_id);

CREATE TABLE userphoto (
    id INTEGER PRIMARY KEY,
    user_id INTEGER REFERENCES user,
    photo_id INTEGER REFERENCES photo,
    status TEXT,
    left_coord INTEGER,
    upper_coord INTEGER,
    right_coord INTEGER,
    lower_coord INTEGER,
    face_file TEXT
);
CREATE INDEX userphoto_user_id_idx ON userphoto(user_id);
CREATE INDEX userphoto_photo_id_idx ON userphoto(photo_id);
