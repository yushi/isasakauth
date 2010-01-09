require 'rubygems'
require 'mechanize'
require 'logger'
require 'test/unit'
require 'net/http'
require 'pp'

class TC_Login < Test::Unit::TestCase
  @@contents_url = 'http://localhost/index.html'
  @@login_url = 'http://localhost/auth/login'
  @@logout_url = 'http://localhost/auth/logout'

  def setup
    @agent = WWW::Mechanize.new { |a| a.log = Logger.new(File.open('/dev/null', 'w')) }
    #@agent = WWW::Mechanize.new { |a| a.log = Logger.new(STDERR) }
    @agent.redirect_ok = false
    @agent.keep_alive = false
  end
  
  # def teardown
  # end

  def test_unauth
    page = @agent.get(@@contents_url)
    assert_equal(page.code, '302')
  end

  def test_login_fail
    page = @agent.get(@@login_url)
    assert_equal(page.code, '302')
    assert_equal('http://localhost/auth/login.html', page.response["location"])
    page = @agent.post(@@login_url, 
                       {
                         'username' =>  'yushi', 
                         'password' =>  'pass'
                       })
    assert_equal(page.code, '302')
    assert_equal('http://localhost/auth/login.html', page.response["location"])
  end

  def test_login_success
    page = @agent.post(@@login_url, 
                       {
                         'username' =>  'yushi', 
                         'password' =>  'hage'
                       })
    assert_equal('http://localhost/index.html', page.response["location"])
    assert_equal('302', page.code)
    assert_match(/^auth=\w{8}-\w{4}-\w{4}-\w{4}-\w{12}; path=\/$/, 
                 page.response['set-cookie'])

    page = @agent.get(@@logout_url)
  end

  def test_authed
    page = @agent.post(@@login_url, 
                       {
                         'username' =>  'yushi', 
                         'password' =>  'hage'
                       })
    assert_equal('http://localhost/index.html', page.response["location"])
    assert_equal('302', page.code)    

    page = @agent.get(@@contents_url)
    assert_equal('200', page.code)    
    page = @agent.get(@@logout_url)
    assert_equal('302', page.code)    

    page = @agent.get(@@contents_url)
    assert_equal(page.code, '302')
  end
end

