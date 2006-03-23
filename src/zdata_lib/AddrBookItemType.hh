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
 * @file AddrBookItemType.hh
 * @brief A specifications file for an object representing an Address Book
 * item.
 * @author Andrew De Ponte
 *
 * A specifications file for a class existing to represent an Address Book
 * item. This data type was created to provide a common data type between the
 * plugins and the Zaurus synchronization application.
 */

#ifndef ADDRBOOKITEMTYPE_H
#define ADDRBOOKITEMTYPE_H

#include "ItemType.hh"

#include <time.h>

#include <string>
#include <list>

/**
 * @class AddrBookItemType
 * @brief A type representing an Address Book item.
 *
 * The AddrBookItemType is a class which represents an Address Book item. It
 * is designed to be used between the Zaurus synchronization application and
 * its plugins so that there is a common object for Address Book items.
 */
class AddrBookItemType : public ItemType {
 public:
    // Define a type that is a list of AddrBookItemTypes.
    typedef std::list<AddrBookItemType> List;

    AddrBookItemType(void);
    ~AddrBookItemType(void);

    // Access functions for the below data members.
    void SetCategory(std::string value);
    std::string GetCategory(void) const;

    void SetFullName(std::string value);
    std::string GetFullName(void) const;

    void SetFullNamePronun(std::string value);
    std::string GetFullNamePronun(void) const;

    void SetTermOfRespect(std::string value);
    std::string GetTermOfRespect(void) const;

    void SetLastName(std::string value);
    std::string GetLastName(void) const;

    void SetFirstName(std::string value);
    std::string GetFirstName(void) const;

    void SetMiddleName(std::string value);
    std::string GetMiddleName(void) const;

    void SetSuffix(std::string value);
    std::string GetSuffix(void) const;

    void SetAlterName(std::string value);
    std::string GetAlterName(void) const;

    void SetLastNamePronun(std::string value);
    std::string GetLastNamePronun(void) const;

    void SetFirstNamePronun(std::string value);
    std::string GetFirstNamePronun(void) const;

    void SetCompany(std::string value);
    std::string GetCompany(void) const;

    void SetCompanyPronun(std::string value);
    std::string GetCompanyPronun(void) const;

    void SetDepartment(std::string value);
    std::string GetDepartment(void) const;

    void SetJobTitle(std::string value);
    std::string GetJobTitle(void) const;

    void SetWorkPhone(std::string value);
    std::string GetWorkPhone(void) const;

    void SetWorkFax(std::string value);
    std::string GetWorkFax(void) const;

    void SetWorkMobile(std::string value);
    std::string GetWorkMobile(void) const;

    void SetWorkState(std::string value);
    std::string GetWorkState(void) const;

    void SetWorkCity(std::string value);
    std::string GetWorkCity(void) const;

    void SetWorkStreet(std::string value);
    std::string GetWorkStreet(void) const;

    void SetWorkZip(std::string value);
    std::string GetWorkZip(void) const;

    void SetWorkCountry(std::string value);
    std::string GetWorkCountry(void) const;

    void SetWorkWebPage(std::string value);
    std::string GetWorkWebPage(void) const;

    void SetOffice(std::string value);
    std::string GetOffice(void) const;

    void SetProfession(std::string value);
    std::string GetProfession(void) const;

    void SetAssistant(std::string value);
    std::string GetAssistant(void) const;

    void SetManager(std::string value);
    std::string GetManager(void) const;

    void SetPager(std::string value);
    std::string GetPager(void) const;

    void SetCellular(std::string value);
    std::string GetCellular(void) const;

    void SetHomePhone(std::string value);
    std::string GetHomePhone(void) const;

    void SetHomeFax(std::string value);
    std::string GetHomeFax(void) const;

    void SetHomeState(std::string value);
    std::string GetHomeState(void) const;

    void SetHomeCity(std::string value);
    std::string GetHomeCity(void) const;

    void SetHomeStreet(std::string value);
    std::string GetHomeStreet(void) const;

    void SetHomeZip(std::string value);
    std::string GetHomeZip(void) const;

    void SetHomeCountry(std::string value);
    std::string GetHomeCountry(void) const;

    void SetHomeWebPage(std::string value);
    std::string GetHomeWebPage(void) const;

    void SetDefaultEmail(std::string value);
    std::string GetDefaultEmail(void) const;

    void SetEmails(std::string value);
    std::string GetEmails(void) const;

    void SetSpouse(std::string value);
    std::string GetSpouse(void) const;

    void SetGender(std::string value);
    std::string GetGender(void) const;

    void SetBirthday(std::string value);
    std::string GetBirthday(void) const;

    void SetAnniversary(std::string value);
    std::string GetAnniversary(void) const;

    void SetNickname(std::string value);
    std::string GetNickname(void) const;

    void SetChildren(std::string value);
    std::string GetChildren(void) const;

    void SetMemo(std::string value);
    std::string GetMemo(void) const;

    void SetGroup(std::string value);
    std::string GetGroup(void) const;

    // Non Access Functions.

 private:
    std::string category;
    std::string fullName;
    std::string fullNamePronun;
    std::string termOfRespect;
    std::string lastName;
    std::string firstName;
    std::string middleName;
    std::string suffix;
    std::string alterName;
    std::string lastNamePronun;
    std::string firstNamePronun;
    std::string company;
    std::string companyPronun;
    std::string department;
    std::string jobTitle;
    std::string workPhone;
    std::string workFax;
    std::string workMobile;
    std::string workState;
    std::string workCity;
    std::string workStreet;
    std::string workZip;
    std::string workCountry;
    std::string workWebPage;
    std::string office;
    std::string profession;
    std::string assistant;
    std::string manager;
    std::string pager;
    std::string cellular;
    std::string homePhone;
    std::string homeFax;
    std::string homeState;
    std::string homeCity;
    std::string homeStreet;
    std::string homeZip;
    std::string homeCountry;
    std::string homeWebPage;
    std::string defaultEmail;
    std::string emails;
    std::string spouse;
    std::string gender;
    std::string birthday;
    std::string anniversary;
    std::string nickname;
    std::string children;
    std::string memo;
    std::string group;
};

#endif
