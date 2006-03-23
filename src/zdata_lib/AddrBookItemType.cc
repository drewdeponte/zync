/*
 * Copyright 2004, 2005 Andrew De Ponte
 * 
 * This file is part of zsrep.
 * 
 * zsrep is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 * 
 * zsrep is distributed in the hopes that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with zsrep; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/**
 * @file AddrBookItemType.cc
 * @brief An implimentation file for the AddrBookItemType class.
 *
 * This type was developed to provide an object to represent an Address Book
 * item. It is designed to be used by the Zaurus synchronization application
 * as well as its plugins so there is a common Address Book item object.
 */

#include "AddrBookItemType.hh"

/**
 * Construct a default AddrBookItemType object.
 *
 * Construct a default AddrBookItemType object with all the basic
 * initialization. 
 */
AddrBookItemType::AddrBookItemType(void) {

}

/**
 * Destruct the AddrBookItemType object.
 *
 * Destruct the AddrBookItemType object by deallocating any dynamically
 * allocated memory. 
 */
AddrBookItemType::~AddrBookItemType(void) {

}

/**
 * Set the category.
 *
 * Set the category of the item.
 * @param value The value to set the category to.
 */
void AddrBookItemType::SetCategory(std::string value) {
    category = value;
}

/**
 * Get the category.
 *
 * Get the category of the item this object represents.
 * @return A string containing the category.
 */
std::string AddrBookItemType::GetCategory(void) const {
    return category;
}

/**
 * Set the Full name.
 *
 * Set the Full name of the item.
 * @param value The value to set the Full name to.
 */
void AddrBookItemType::SetFullName(std::string value) {
    fullName = value;
}

/**
 * Get the Full name.
 *
 * Get the Full name of the item this object represents.
 * @return A string containing the Full name.
 */
std::string AddrBookItemType::GetFullName(void) const {
    return fullName;
}

/**
 * Set the Full name pronunciation.
 *
 * Set the Full name pronunciation of the item.
 * @param value The value to set the Full name pronunciation to.
 */
void AddrBookItemType::SetFullNamePronun(std::string value) {
    fullNamePronun = value;
}

/**
 * Get the Full name pronunciation.
 *
 * Get the Full name pronunciation of the item this object represents.
 * @return A string containing the Full name pronunciation.
 */
std::string AddrBookItemType::GetFullNamePronun(void) const {
    return fullNamePronun;
}

/**
 * Set the Term of respect.
 *
 * Set the Term of respect of the item.
 * @param value The value to set the Term of respect to.
 */
void AddrBookItemType::SetTermOfRespect(std::string value) {
    termOfRespect = value;
}

/**
 * Get the Term of respect.
 *
 * Get the term of respect of the item this object represents.
 * @return A string containing the Term of respect.
 */
std::string AddrBookItemType::GetTermOfRespect(void) const {
    return termOfRespect;
}

/**
 * Set the Last Name.
 *
 * Set the Last Name of the item.
 * @param value The value to set the Last Name to.
 */
void AddrBookItemType::SetLastName(std::string value) {
    lastName = value;
}

/**
 * Get the Last Name.
 *
 * Get the Last Name of the item this object represents.
 * @return A string containing the Last Name.
 */
std::string AddrBookItemType::GetLastName(void) const {
    return lastName;
}

/**
 * Set the First Name.
 *
 * Set the First Name of the item.
 * @param value The value to set the First Name to.
 */
void AddrBookItemType::SetFirstName(std::string value) {
    firstName = value;
}

/**
 * Get the First Name.
 *
 * Get the First Name of the item this object represents.
 * @return A string containing the First Name.
 */
std::string AddrBookItemType::GetFirstName(void) const {
    return firstName;
}

/**
 * Set the Middle Name.
 *
 * Set the Middle Name of the item.
 * @param value The value to set the Middle Name to.
 */
void AddrBookItemType::SetMiddleName(std::string value) {
    middleName = value;
}

/**
 * Get the Middle Name.
 *
 * Get the Middle Name of the item this object represents.
 * @return A string containing the Middle Name.
 */
std::string AddrBookItemType::GetMiddleName(void) const {
    return middleName;
}

/**
 * Set the Suffix.
 *
 * Set the Suffix of the item.
 * @param value The value to set the Suffix to.
 */
void AddrBookItemType::SetSuffix(std::string value) {
    suffix = value;
}

/**
 * Get the Suffix.
 *
 * Get the Suffix of the item this object represents.
 * @return A string containing the Suffix.
 */
std::string AddrBookItemType::GetSuffix(void) const {
    return suffix;
}

/**
 * Set the Alternate Name.
 *
 * Set the Alternate Name of the item.
 * @param value The value to set the Alternate Name to.
 */
void AddrBookItemType::SetAlterName(std::string value) {
    alterName = value;
}

/**
 * Get the Alternate Name.
 *
 * Get the Alternate Name of the item this object represents.
 * @return A string containing the Alternate Name.
 */
std::string AddrBookItemType::GetAlterName(void) const {
    return alterName;
}

/**
 * Set the Last Name Pronunciation.
 *
 * Set the Last Name Pronunciation of the item.
 * @param value The value to set the Last Name Pronunciation to.
 */
void AddrBookItemType::SetLastNamePronun(std::string value) {
    lastNamePronun = value;
}

/**
 * Get the Last Name Pronunciation.
 *
 * Get the Last Name Pronunciation of the item this object represents.
 * @return A string containing the Last Name Pronunciation.
 */
std::string AddrBookItemType::GetLastNamePronun(void) const {
    return lastNamePronun;
}

/**
 * Set the First Name Pronunciation.
 *
 * Set the First Name Pronunciation of the item.
 * @param value The value to set the First Name Pronunciation to.
 */
void AddrBookItemType::SetFirstNamePronun(std::string value) {
    firstNamePronun = value;
}

/**
 * Get the First Name Pronunciation.
 *
 * Get the First Name Pronunciation of the item this object represents.
 * @return A string containing the First Name Pronunciation.
 */
std::string AddrBookItemType::GetFirstNamePronun(void) const {
    return firstNamePronun;
}

/**
 * Set the Company.
 *
 * Set the Company of the item.
 * @param value The value to set the Company to.
 */
void AddrBookItemType::SetCompany(std::string value) {
    company = value;
}

/**
 * Get the Company.
 *
 * Get the Company of the item this object represents.
 * @return A string containing the Company.
 */
std::string AddrBookItemType::GetCompany(void) const {
    return company;
}

/**
 * Set the Company Pronunciation.
 *
 * Set the Company Pronunciation of the item.
 * @param value The value to set the Company Pronunciation to.
 */
void AddrBookItemType::SetCompanyPronun(std::string value) {
    companyPronun = value;
}

/**
 * Get the Company Pronunciation.
 *
 * Get the Company Pronunciation of the item this object represents.
 * @return A string containing the Company Pronunciation.
 */
std::string AddrBookItemType::GetCompanyPronun(void) const {
    return companyPronun;
}

/**
 * Set the Department.
 *
 * Set the Department of the item.
 * @param value The value to set the Department to.
 */
void AddrBookItemType::SetDepartment(std::string value) {
    department = value;
}

/**
 * Get the Department.
 *
 * Get the Department of the item this object represents.
 * @return A string containing the Department.
 */
std::string AddrBookItemType::GetDepartment(void) const {
    return department;
}

/**
 * Set the Job Title.
 *
 * Set the Job Title of the item.
 * @param value The value to set the Job Title to.
 */
void AddrBookItemType::SetJobTitle(std::string value) {
    jobTitle = value;
}

/**
 * Get the Job Title.
 *
 * Get the Job Title  of the item this object represents.
 * @return A string containing the Job Title.
 */
std::string AddrBookItemType::GetJobTitle(void) const {
    return jobTitle;
}

/**
 * Set the Work Phone.
 *
 * Set the Work Phone of the item.
 * @param value The value to set the Work Phone to.
 */
void AddrBookItemType::SetWorkPhone(std::string value) {
    workPhone = value;
}

/**
 * Get the Work Phone.
 *
 * Get the Work Phone of the item this object represents.
 * @return A string containing the Work Phone.
 */
std::string AddrBookItemType::GetWorkPhone(void) const {
    return workPhone;
}

/**
 * Set the Work Fax.
 *
 * Set the Work Fax of the item.
 * @param value The value to set the Work Fax to.
 */
void AddrBookItemType::SetWorkFax(std::string value) {
    workFax = value;
}

/**
 * Get the Work Fax.
 *
 * Get the Work Fax of the item this object represents.
 * @return A string containing the Work Fax.
 */
std::string AddrBookItemType::GetWorkFax(void) const {
    return workFax;
}

/**
 * Set the Work Mobile.
 *
 * Set the Work Mobile of the item.
 * @param value The value to set the Work Mobile to.
 */
void AddrBookItemType::SetWorkMobile(std::string value) {
    workMobile = value;
}

/**
 * Get the Work Mobile.
 *
 * Get the Work Mobile of the item this object represents.
 * @return A string containing the Work Mobile.
 */
std::string AddrBookItemType::GetWorkMobile(void) const {
    return workMobile;
}

/**
 * Set the Work State.
 *
 * Set the Work State of the item.
 * @param value The value to set the Work State to.
 */
void AddrBookItemType::SetWorkState(std::string value) {
    workState = value;
}

/**
 * Get the Work State.
 *
 * Get the Work State of the item this object represents.
 * @return A string containing the Work Stat.
 */
std::string AddrBookItemType::GetWorkState(void) const {
    return workState;
}

/**
 * Set the Work City.
 *
 * Set the Work City of the item.
 * @param value The value to set the Work City to.
 */
void AddrBookItemType::SetWorkCity(std::string value) {
    workCity = value;
}

/**
 * Get the Work City.
 *
 * Get the Work City of the item this object represents.
 * @return A string containing the Work City.
 */
std::string AddrBookItemType::GetWorkCity(void) const {
    return workCity;
}

/**
 * Set the Work Street.
 *
 * Set the Work Street of the item.
 * @param value The value to set the Work Street to.
 */
void AddrBookItemType::SetWorkStreet(std::string value) {
    workStreet = value;
}

/**
 * Get the Work Street.
 *
 * Get the Work Street of the item this object represents.
 * @return A string containing the Work Street.
 */
std::string AddrBookItemType::GetWorkStreet(void) const {
    return workStreet;
}

/**
 * Set the Work Zip.
 *
 * Set the Work Zip of the item.
 * @param value The value to set the Work Zip to.
 */
void AddrBookItemType::SetWorkZip(std::string value) {
    workZip = value;
}

/**
 * Get the Work Zip.
 *
 * Get the Work Zip of the item this object represents.
 * @return A string containing the Work Zip.
 */
std::string AddrBookItemType::GetWorkZip(void) const {
    return workZip;
}

/**
 * Set the Work Country.
 *
 * Set the Work Country of the item.
 * @param value The value to set the Work Country to.
 */
void AddrBookItemType::SetWorkCountry(std::string value) {
    workCountry = value;
}

/**
 * Get the Work Country.
 *
 * Get the Work Country of the item this object represents.
 * @return A string containing the Work Country.
 */
std::string AddrBookItemType::GetWorkCountry(void) const {
    return workCountry;
}

/**
 * Set the Work Web Page.
 *
 * Set the Work Web Page of the item.
 * @param value The value to set the Work Web Page to.
 */
void AddrBookItemType::SetWorkWebPage(std::string value) {
    workWebPage = value;
}

/**
 * Get the Work Web Page.
 *
 * Get the Work Web Page of the item this object represents.
 * @return A string containing the Work Web Page.
 */
std::string AddrBookItemType::GetWorkWebPage(void) const {
    return workWebPage;
}

/**
 * Set the Office.
 *
 * Set the Office of the item.
 * @param value The value to set the Office to.
 */
void AddrBookItemType::SetOffice(std::string value) {
    office = value;
}

/**
 * Get the Office.
 *
 * Get the Office of the item this object represents.
 * @return A string containing the Office.
 */
std::string AddrBookItemType::GetOffice(void) const {
    return office;
}

/**
 * Set the Profession.
 *
 * Set the Profession of the item.
 * @param value The value to set the Profession to.
 */
void AddrBookItemType::SetProfession(std::string value) {
    profession = value;
}

/**
 * Get the Profession.
 *
 * Get the Profession of the item this object represents.
 * @return A string containing the Profession.
 */
std::string AddrBookItemType::GetProfession(void) const {
    return profession;
}

/**
 * Set the Assistant.
 *
 * Set the Assistant of the item.
 * @param value The value to set the Assistant to.
 */
void AddrBookItemType::SetAssistant(std::string value) {
    assistant = value;
}

/**
 * Get the Assistant.
 *
 * Get the Assistant of the item this object represents.
 * @return A string containing the Assistant.
 */
std::string AddrBookItemType::GetAssistant(void) const {
    return assistant;
}

/**
 * Set the Manager.
 *
 * Set the Manager of the item.
 * @param value The value to set the Manager to.
 */
void AddrBookItemType::SetManager(std::string value) {
    manager = value;
}

/**
 * Get the Manager.
 *
 * Get the Manager of the item this object represents.
 * @return A string containing the Manager.
 */
std::string AddrBookItemType::GetManager(void) const {
    return manager;
}

/**
 * Set the Pager.
 *
 * Set the Pager of the item.
 * @param value The value to set the Pager to.
 */
void AddrBookItemType::SetPager(std::string value) {
    pager = value;
}

/**
 * Get the Pager.
 *
 * Get the Pager of the item this object represents.
 * @return A string containing the Pager.
 */
std::string AddrBookItemType::GetPager(void) const {
    return pager;
}

/**
 * Set the Cellular.
 *
 * Set the Cellular of the item.
 * @param value The value to set the Cellular to.
 */
void AddrBookItemType::SetCellular(std::string value) {
    cellular = value;
}

/**
 * Get the Cellular.
 *
 * Get the Cellular of the item this object represents.
 * @return A string containing the Cellular.
 */
std::string AddrBookItemType::GetCellular(void) const {
    return cellular;
}

/**
 * Set the Home Phone.
 *
 * Set the Home Phone of the item.
 * @param value The value to set the Home Phone to.
 */
void AddrBookItemType::SetHomePhone(std::string value) {
    homePhone = value;
}

/**
 * Get the Home Phone.
 *
 * Get the Home Phone of the item this object represents.
 * @return A string containing the Home Phone.
 */
std::string AddrBookItemType::GetHomePhone(void) const {
    return homePhone;
}

/**
 * Set the Home Fax.
 *
 * Set the Home Fax of the item.
 * @param value The value to set the Home Fax to.
 */
void AddrBookItemType::SetHomeFax(std::string value) {
    homeFax = value;
}

/**
 * Get the Home Fax.
 *
 * Get the Home Fax of the item this object represents.
 * @return A string containing the Home Fax.
 */
std::string AddrBookItemType::GetHomeFax(void) const {
    return homeFax;
}

/**
 * Set the Home State.
 *
 * Set the Home State of the item.
 * @param value The value to set the Home State to.
 */
void AddrBookItemType::SetHomeState(std::string value) {
    homeState = value;
}

/**
 * Get the Home State.
 *
 * Get the Home State of the item this object represents.
 * @return A string containing the Home State.
 */
std::string AddrBookItemType::GetHomeState(void) const {
    return homeState;
}

/**
 * Set the Home City.
 *
 * Set the Home City of the item.
 * @param value The value to set the Home City to.
 */
void AddrBookItemType::SetHomeCity(std::string value) {
    homeCity = value;
}

/**
 * Get the Home City.
 *
 * Get the Home City of the item this object represents.
 * @return A string containing the Home City.
 */
std::string AddrBookItemType::GetHomeCity(void) const {
    return homeCity;
}

/**
 * Set the Home Street.
 *
 * Set the Home Street of the item.
 * @param value The value to set the Home Street to.
 */
void AddrBookItemType::SetHomeStreet(std::string value) {
    homeStreet = value;
}

/**
 * Get the Home Street.
 *
 * Get the Home Street of the item this object represents.
 * @return A string containing the Home Street.
 */
std::string AddrBookItemType::GetHomeStreet(void) const {
    return homeStreet;
}

/**
 * Set the Home Zip.
 *
 * Set the Home Zip of the item.
 * @param value The value to set the Home Zip to.
 */
void AddrBookItemType::SetHomeZip(std::string value) {
    homeZip = value;
}

/**
 * Get the Home Zip.
 *
 * Get the Home Zip of the item this object represents.
 * @return A string containing the Home Zip.
 */
std::string AddrBookItemType::GetHomeZip(void) const {
    return homeZip;
}

/**
 * Set the Home Country.
 *
 * Set the Home Country of the item.
 * @param value The value to set the Home Country to.
 */
void AddrBookItemType::SetHomeCountry(std::string value) {
    homeCountry = value;
}

/**
 * Get the Home Country.
 *
 * Get the Home Country of the item this object represents.
 * @return A string containing the Home Country.
 */
std::string AddrBookItemType::GetHomeCountry(void) const {
    return homeCountry;
}

/**
 * Set the Home Web Page.
 *
 * Set the Home Web Page of the item.
 * @param value The value to set the Home Web Page to.
 */
void AddrBookItemType::SetHomeWebPage(std::string value) {
    homeWebPage = value;
}

/**
 * Get the Home Web Page.
 *
 * Get the Home Web Page of the item this object represents.
 * @return A string containing the Home Web Page.
 */
std::string AddrBookItemType::GetHomeWebPage(void) const {
    return homeWebPage;
}

/**
 * Set the Default Email.
 *
 * Set the Default Email of the item.
 * @param value The value to set the Default Email to.
 */
void AddrBookItemType::SetDefaultEmail(std::string value) {
    defaultEmail = value;
}

/**
 * Get the Default Email.
 *
 * Get the Default Email of the item this object represents.
 * @return A string containing the Default Email.
 */
std::string AddrBookItemType::GetDefaultEmail(void) const {
    return defaultEmail;
}

/**
 * Set the Emails.
 *
 * Set the Emails of the item. This can be a space seperated list of emails,
 * the first of which is the Default Email.
 * @param value The value to set the Emails to.
 */
void AddrBookItemType::SetEmails(std::string value) {
    emails = value;
}

/**
 * Get the Emails.
 *
 * Get the Emails of the item this object represents. This can be a space
 * seperated list of emails, the first of which is the Default Email.
 * @return A string containing the Emails.
 */
std::string AddrBookItemType::GetEmails(void) const {
    return emails;
}

/**
 * Set the Spouse.
 *
 * Set the Spouse of the item.
 * @param value The value to set the Spouse to.
 */
void AddrBookItemType::SetSpouse(std::string value) {
    spouse = value;
}

/**
 * Get the Spouse.
 *
 * Get the Spouse of the item this object represents.
 * @return A string containing the Spouse.
 */
std::string AddrBookItemType::GetSpouse(void) const {
    return spouse;
}

/**
 * Set the Gender.
 *
 * Set the Gender of the item.
 * @param value The value to set the Gender to.
 */
void AddrBookItemType::SetGender(std::string value) {
    gender = value;
}

/**
 * Get the Gender.
 *
 * Get the Gender of the item this object represents.
 * @return A string containing the Gender.
 */
std::string AddrBookItemType::GetGender(void) const {
    return gender;
}

/**
 * Set the Birthday.
 *
 * Set the Birthday of the item.
 * @param value The value to set the Birthday to.
 */
void AddrBookItemType::SetBirthday(std::string value) {
    birthday = value;
}

/**
 * Get the Birthday.
 *
 * Get the Birthday of the item this object represents.
 * @return A string containing the Birthday.
 */
std::string AddrBookItemType::GetBirthday(void) const {
    return birthday;
}

/**
 * Set the Anniversary.
 *
 * Set the Anniversary of the item.
 * @param value The value to set the Anniversary to.
 */
void AddrBookItemType::SetAnniversary(std::string value) {
    anniversary = value;
}

/**
 * Get the Anniversary.
 *
 * Get the Anniversary of the item this object represents.
 * @return A string containing the Anniversary.
 */
std::string AddrBookItemType::GetAnniversary(void) const {
    return anniversary;
}

/**
 * Set the Nickname.
 *
 * Set the Nickname of the item.
 * @param value The value to set the Nickname to.
 */
void AddrBookItemType::SetNickname(std::string value) {
    nickname = value;
}

/**
 * Get the Nickname.
 *
 * Get the Nickname of the item this object represents.
 * @return A string containing the Nickname.
 */
std::string AddrBookItemType::GetNickname(void) const {
    return nickname;
}

/**
 * Set the Children.
 *
 * Set the Children of the item.
 * @param value The value to set the Children to.
 */
void AddrBookItemType::SetChildren(std::string value) {
    children = value;
}

/**
 * Get the Children.
 *
 * Get the Children of the item this object represents.
 * @return A string containing the Children.
 */
std::string AddrBookItemType::GetChildren(void) const {
    return children;
}

/**
 * Set the Memo.
 *
 * Set the Memo of the item.
 * @param value The value to set the Memo to.
 */
void AddrBookItemType::SetMemo(std::string value) {
    memo = value;
}

/**
 * Get the Memo.
 *
 * Get the Memo of the item this object represents.
 * @return A string containing the Memo.
 */
std::string AddrBookItemType::GetMemo(void) const {
    return memo;
}

/**
 * Set the Group.
 *
 * Set the Group of the item.
 * @param value The value to set the Group to.
 */
void AddrBookItemType::SetGroup(std::string value) {
    group = value;
}

/**
 * Get the Group.
 *
 * Get the Group of the item this object represents.
 * @return A string containing the Group.
 */
std::string AddrBookItemType::GetGroup(void) const {
    return group;
}
