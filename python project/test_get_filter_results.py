import unittest
import twitterverse_functions as tf

data = {'tomCruise': {'name': 'Tom Cruise', 'bio': 'Official TomCruise.com crew tweets. We love you guys!\nVisit us at Facebook!', 'location': 'Los Angeles, CA', 'web': 'http://www.tomcruise.com', 'following': ['katieH', 'NicoleKidman']}, 'PerezHilton': {'name': 'Perez Hilton', 'bio': 'Perez Hilton is the creator and writer of one of the most famous websites\nin the world. And he also loves music - a lot!', 'location': 'Hollywood, California', 'web': 'http://www.PerezH...', 'following': ['tomCruise', 'katieH', 'NicoleKidman']}, 'katieH': {'name': 'Katie Holmes', 'bio': '', 'location': '', 'web': 'www.tomkat.com', 'following': []}, 'NicoleKidman': {'name': 'Nicole Kidman', 'bio': "At my house celebrating Halloween! I Know Haven't been on like \nyears So Sorry,Be safe And have fun tonight", 'location': 'Oz', 'web': '', 'following': []}, 'tomfan': {'name': 'Chris Calderone', 'bio': 'Tom Cruise is the best actor in Hollywood!', 'location': 'Houston, Texas', 'web': '', 'following': ['tomCruise']}, 'p': {'name': 'Mme Clavell', 'bio': 'I love winter, snow and ice.', 'location': 'Paris, France', 'web': '', 'following': ['NicoleKidman']}, 'q': {'name': 'Quincy Q', 'bio': '', 'location': 'Port Coquitlam, BC', 'web': 'http://www.something.com', 'following': ['NicoleKidman']}, 'a': {'name': 'Alex D', 'bio': 'Love the outdoors, even \nin the rain.', 'location': 'Abbottsford, British Columbia', 'web': 'www.abbotsford.ca', 'following': ['tomfan']}, 'b': {'name': 'Benny Lewis', 'bio': '', 'location': 'Bankok', 'web': '', 'following': ['tomfan']}, 'x': {'name': 'Xavier', 'bio': '', 'location': 'Xerox Parc', 'web': 'www.xerox.com', 'following': ['c']}, 'Y': {'name': 'Yousef', 'bio': 'Welcome to Yarmouth, Nova Scotia - rich in history and culture, home to year-round festivals and live entertainment, and the perfect place for your next special getaway. We look forward to meeting you!', 'location': 'Yarmouth, Nova Scotia', 'web': 'yarmouthonline.ca', 'following': ['c']}, 'z': {'name': 'Zoya Zorich', 'bio': '', 'location': '', 'web': '', 'following': ['b']}, 'c': {'name': 'The Captain', 'bio': '', 'location': 'Kansas', 'web': 'kellogs.com', 'following': []}}

class TestGetFilterResults(unittest.TestCase):
    '''Your unittests here'''
    
    def test_get_filter_locationincludes(self):
        """Test get_filter_results when the result includes users that satisfy 
        the filter requirement of LOCATION_INCLUDES.
        """
        usernames = ['tomCruise', 'PerezHilton']
        f_d = {'location-includes': 'Los Angeles, CA'}
        actual = tf.get_filter_results(data, usernames, f_d)
        expected = ['tomCruise']
        self.assertEqual(expected, actual)

    def test_get_filter_nameincludes(self):
        """Test get_filter_results when the result includes users that satisfy 
        the filter requirement of NAME_INCLUDES.
        """        
        usernames = ['tomCruise', 'NicoleKidman', 'a']
        f_d = {'name-includes': 'C'}
        actual = tf.get_filter_results(data, usernames, f_d)
        expected = ['tomCruise', 'NicoleKidman']
        self.assertEqual(expected, actual)

    def test_get_filter_following(self):
        """Test get_filter_results when the result includes users that satisfy 
        the filter requirement of FOLLOWING.
        """
        usernames = ['tomCruise', 'NicoleKidman']
        f_d = {'following': 'katieH'}
        actual = tf.get_filter_results(data, usernames, f_d)
        expected = ['tomCruise']
        self.assertEqual(expected, actual)

    def test_get_filter_follower(self):
        """Test get_filter_results when the result includes users that satisfy 
        the filter requirement of FOLLOWERS.
        """
        usernames = ['c', 'tomCruise', 'NicoleKidman']
        f_d = {'followers': 'tomfan'}
        actual = tf.get_filter_results(data, usernames, f_d)
        expected = ['tomCruise']
        self.assertEqual(expected, actual)
    
    def test_get_filter_empty(self):
        """Test get_filter_results when the filter requirment is empty.
        """
        usernames = ['a', 'b']
        f_d = {}
        actual = tf.get_filter_results(data, usernames, f_d)
        expected = ['a', 'b']
        self.assertEqual(expected, actual)
    
    def test_get_filter_combine(self):
        """Test get_filter_results when the result includes users that satisfy 
        all the filter requirement at the same time.
        """
        usernames = ['tomCruise', 'tomfan', 'PerezHilton', ]
        f_d = {'location-includes': 'A', 'name-includes': 'C',
               'following': 'katieH', 'follower': 'tomfan'}
        actual = tf.get_filter_results(data, usernames, f_d)
        expected = ['tomCruise']
        self.assertEqual(expected, actual)
  
if __name__ == '__main__':
    unittest.main(exit=False)

