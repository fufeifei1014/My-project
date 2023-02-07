"""CSC108/A08: Fall 2020 -- Assignment 3: Twitterverse

This code is provided solely for the personal and private use of
students taking the CSC108 course at the University of
Toronto. Copying for purposes other than this use is expressly
prohibited. All forms of distribution of this code, whether as given
or with any changes, are expressly prohibited.

All of the files in this directory and all subdirectories are:
Copyright (c) 2020 Mario Badr, Jennifer Campbell, Tom Fairgrieve,
Diane Horton, Michael Liut, Jacqueline Smith, and Anya Tafliovich.

"""

from typing import Callable, List, TextIO
from constants import (TwitterverseDict, SearchDict, FilterDict,
                       PresentDict, QueryDict)
from constants import (NAME, LOCATION, WEB, BIO, FOLLOWING, USERNAME,
                       OPERATIONS, FOLLOWER, FOLLOWERS, NAME_INCLUDES,
                       LOCATION_INCLUDES, SORT_BY, FORMAT, SEARCH,
                       FILTER, PRESENT, POPULARITY, END, ENDBIO, LONG)


HANDOUT_DATA = {
    'tomCruise': {
        'name': 'Tom Cruise',
        'bio': ('Official TomCruise.com crew tweets. We love you guys!\n' +
                'Visit us at Facebook!'),
        'location': 'Los Angeles, CA',
        'web': 'http://www.tomcruise.com',
        'following': ['katieH', 'NicoleKidman']},
    'PerezHilton': {
        'name': 'Perez Hilton',
        'bio': ('Perez Hilton is the creator and writer of one of the most ' +
                'famous websites\nin the world. And he also loves music -' +
                'a lot!'),
        'location': 'Hollywood, California',
        'web': 'http://www.PerezH...',
        'following': ['tomCruise', 'katieH', 'NicoleKidman']}}

HANDOUT_QUERY = {
    'SEARCH': {'username': 'tomCruise',
               'operations': ['following', 'followers']},
    'FILTER': {'following': 'katieH'},
    'PRESENT': {'sort-by': 'username',
                'format': 'short'}
}

############################################################################
# helper function


def remove_repeated(data_list: List[str]) -> List[str]:
    """Return the data_list by removeing repeated str in it.
    >>> remove_repeated(['a', 'a'])
    ['a']
    """
    new_list = []
    for item in data_list:
        if not item in new_list:
            new_list.append(item)
    return new_list


def store_following_data(username: [str],
                         twitter_data: TwitterverseDict) -> List[str]:
    """Return the usersnames_list by adding the username which is in the
    twitter_data in it.
    """
    new_list = []
    for name in twitter_data[username][FOLLOWING]:
        new_list.append(name)
    return new_list


def store_follower_data(username: [str],
                        twitter_data: TwitterverseDict) -> List[str]:
    """Return the usersnames_list by adding the username which is in the
    twitter_data in it.
    """
    new_list = []
    for name in all_followers(twitter_data, username):
        new_list.append(name)
    return new_list


def append_list(check: bool, item: str) -> List[str]:
    """Return a list by adding item in it when check is True.
    """
    new_list = []
    if check:
        new_list.append(item)
    return new_list

############################################################################
# Provided helper functions
############################################################################


def tweet_sort(twitter_data: TwitterverseDict,
               usernames: List[str],
               sort_spec: str) -> None:
    """Sort usernames based on the sorting specification in sort_spec
    using the data in twitter_data.

    >>> usernames = ['tomCruise', 'PerezHilton']
    >>> tweet_sort(HANDOUT_DATA, usernames, 'username')
    >>> usernames == ['PerezHilton', 'tomCruise']
    True
    >>> tweet_sort(HANDOUT_DATA, usernames, 'popularity')
    >>> usernames == ['tomCruise', 'PerezHilton']
    True
    >>> tweet_sort(HANDOUT_DATA, usernames, 'name')
    >>> usernames == ['PerezHilton', 'tomCruise']
    True

    """

    usernames.sort()  # sort by username first
    if sort_spec in SORT_FUNCS:
        SORT_FUNCS[sort_spec](twitter_data, usernames)


def by_popularity(twitter_data: TwitterverseDict, usernames: List[str]) -> None:
    """Sort usernames in descending order based on popularity (number of
    users that follow a gien user) in twitter_data.

    >>> usernames = ['PerezHilton', 'tomCruise']
    >>> by_popularity(HANDOUT_DATA, usernames)
    >>> usernames == ['tomCruise', 'PerezHilton']
    True

    """

    def get_popularity(username: str) -> int:
        return len(all_followers(twitter_data, username))

    usernames.sort(key=get_popularity, reverse=True)


def by_name(twitter_data: TwitterverseDict, usernames: List[str]) -> None:
    """Sort usernames in ascending order based on name in twitter_data.

    >>> usernames = ['tomCruise', 'PerezHilton']
    >>> by_name(HANDOUT_DATA, usernames)
    >>> usernames == ['PerezHilton', 'tomCruise']
    True

    """

    def get_name(username: str) -> str:
        return twitter_data.get(username, {}).get(NAME, '')

    usernames.sort(key=get_name)


def format_report(twitter_data: TwitterverseDict,
                  usernames: List[str],
                  format_spec: str) -> str:
    """Return a string representing usernames presented as specified by
    the format specification format_spec.

    Precondition: each username in usernames is in twitter_data
    """

    if format_spec == LONG:
        result = '-' * 10 + '\n'
        for user in usernames:
            result += format_details(twitter_data, user)
            result += '-' * 10 + '\n'
        return result
    return str(usernames)


def format_details(twitter_data: TwitterverseDict, username: str) -> str:
    """Return a string representing the long format of username's info in
    twitter_data.

    Precondition: username is in twitter_data
    """

    user_data = twitter_data[username]
    return ("{}\nname: {}\nlocation: {}\nwebsite: {}\nbio:\n{}\n" +
            "following: {}\n").format(username, user_data[NAME],
                                      user_data[LOCATION],
                                      user_data[WEB], user_data[BIO],
                                      user_data[FOLLOWING])


def process_data(twitter_file: TextIO) -> TwitterverseDict:
    """Return the data as a TwitterverseDict by reading the data in the
    data file.
    """
    twitter_list = twitter_file.read().split('\n')
    user_info = {}
    i = 0
    while i < len(twitter_list) - 1:
        key = twitter_list[i]
        name = twitter_list[i + 1]
        loc = twitter_list[i + 2]
        web = twitter_list[i + 3]
        bio = ""
        i += 4
        while twitter_list[i] != 'ENDBIO':
            bio += twitter_list[i]
            bio += '\n'
            i += 1
            if twitter_list[i] == 'ENDBIO':
                bio = bio[:-1]
        follow = []
        i += 1
        while twitter_list[i] != 'END':
            follow.append(twitter_list[i])
            i += 1
        user_info[key] = {NAME: name, BIO: bio, LOCATION: loc,
                          WEB: web, FOLLOWING: follow}
        i += 1
    return user_info


def process_query(query_file: TextIO) -> QueryDict:
    """Return the dictionary of query in the QueryDict format by reading the
    query_file.
    """

    query_list = query_file.read().strip().split('\n')
    username = query_list[1]
    operations = []
    filtering = {}
    present = {}
    filter_data = []
    present_data = []
    i = 2
    while query_list[i] != FILTER:
        operations.append(query_list[i])
        i += 1
    i += 1
    while query_list[i] != PRESENT:
        filter_data = query_list[i].split()
        filtering[filter_data[0]] = filter_data[1]
        i += 1
    i += 1
    while i < len(query_list):
        present_data = query_list[i].split()
        present[present_data[0]] = present_data[1]
        i += 1
    query_dict = {SEARCH: {USERNAME: username,
                           OPERATIONS: operations},
                  FILTER: filtering,
                  PRESENT: present}
    return query_dict


def all_followers(twitter_data: TwitterverseDict, name: str) -> List[str]:
    """Return the following's usernames of name from the twitter_data
    as a list.
    >>> all_followers(process_data(open('data.txt')),'tomfan')
    ['a', 'b']
    >>> all_followers(process_data(open('data.txt')),'PerezHilton')
    []
    """
    followers_list = []
    for user in twitter_data:
        if name in twitter_data[user][FOLLOWING]:
            followers_list.append(user)
    return followers_list


def get_search_results(twitter_data: TwitterverseDict,
                       search_dict: SearchDict) -> List[str]:
    """Return a list of strings representing usernames in twitter_data which
    followed by the search method of search_dict.
    >>> data = open('diagram_data.txt')
    >>> query = open('diagram_query.txt')
    >>> a = process_data(data)
    >>> b = process_query(query)
    >>> c = b[SEARCH]
    >>> get_search_results(a,c)
    ['tomCruise']
    >>> data = open('data.txt')
    >>> query = open('query1.txt')
    >>> a = process_data(data)
    >>> b = process_query(query)
    >>> c = b[SEARCH]
    >>> get_search_results(a,c)
    ['NicoleKidman', 'katieH']
    """
    new_list = []
    usersnames_list = [search_dict[USERNAME]]
    for operation in search_dict[OPERATIONS]:
        for username in usersnames_list:
            if operation == FOLLOWING:
                new_list = store_following_data(username,
                                                twitter_data)
            elif operation == FOLLOWERS:
                new_list = store_follower_data(username,
                                               twitter_data)
        usersnames_list = new_list[:]
        new_list = []
    required_list = remove_repeated(usersnames_list)
    required_list.sort()
    return required_list


def get_filter_results(data: TwitterverseDict,
                       user_list: List[str], filt: FilterDict) -> List[str]:
    """Return the list of usernames from data by applying the specified filters
    in filt to the user_list to determine which usernames to keep.
    >>> a = process_query(open('query1.txt'))
    >>> b = a[FILTER]
    >>> get_filter_results(HANDOUT_DATA, ['tomCruise'], b)
    ['tomCruise']
    >>> a = process_query(open('query3.txt'))
    >>> b = a[FILTER]
    >>> get_filter_results(HANDOUT_DATA, ['tomCruise','PerezHilton'], b)
    ['tomCruise', 'PerezHilton']
    """
    rest = []
    new_list = []
    for user in user_list:
        check = True
        dic = data[user]
        if NAME_INCLUDES in filt and not filt[NAME_INCLUDES].lower()\
           in dic[NAME].lower():
            check = False
        if LOCATION_INCLUDES in filt and not filt[LOCATION_INCLUDES].lower()\
           in dic[LOCATION].lower():
            check = False
        if FOLLOWING in filt and not filt[FOLLOWING] in dic[FOLLOWING]:
            check = False
        if FOLLOWERS in filt and filt[FOLLOWERS] not in\
           all_followers(data, user):
            check = False
        rest = append_list(check, user)
        new_list.extend(rest)
    return new_list


def get_present_string(twitter_data: TwitterverseDict,
                       lis: List[str], present_dict: PresentDict) -> List[str]:
    """Return the list of username and twitter_data in list by processing the
    format of present_dict.
    >>> user = ['PerezHilton', 'KatieH']
    >>> get_present_string(HANDOUT_DATA, user, HANDOUT_QUERY[PRESENT])
    "['KatieH', 'PerezHilton']"
    >>> user = ['tomCruise', 'NicoleKidman']
    >>> get_present_string(HANDOUT_DATA, user, HANDOUT_QUERY[PRESENT])
    "['NicoleKidman', 'tomCruise']"
    """
    for operations in present_dict.keys():
        if operations == SORT_BY:
            tweet_sort(twitter_data, lis, present_dict[SORT_BY])
        elif operations == FORMAT:
            user_data = format_report(twitter_data, lis, present_dict[FORMAT])
    return user_data

############################################################################


SORT_FUNCS = {POPULARITY: by_popularity,
              NAME: by_name}


if __name__ == '__main__':
    import doctest
    doctest.testmod()
