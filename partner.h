class Partner
{
private:
    const char* name;
    const char* organisation;
    unsigned long phone;
    const char* email;
public:
    Partner(const char* name_new, const char* org, const char* phone_new, const char* email_new);
    Partner(const Partner& other);  
    Partner();
    ~Partner();

    const char* getName();
    const char* getOrg();
    const char* getEmail();
    const unsigned long getPhone();
    
    bool operator == (const Partner& other) const;
    bool operator != (const Partner& other) const;
    bool operator > (const Partner& other) const;
    bool operator < (const Partner& other) const;
    bool operator >= (const Partner& other) const;
    bool operator <= (const Partner& other) const;

    Partner& operator=(const Partner& other); 

    bool setAttr(const char* name_new=nullptr, const char* org=nullptr, const char* phone_new=nullptr, const char* email_new=nullptr);
};