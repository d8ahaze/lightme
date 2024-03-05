// const URL = "http://192.168.1.111:8080/api/getdiff" -- TESTING
const URL = "https://zxcvx.cc:8080/api/getdiff"

async function get_site_keywords()
{
	// user's website
	var uws = document.getElementById("user-website").value;
	console.log(uws);
	const pararr = {link: uws};
	const params = new URLSearchParams(pararr);
	const response = await fetch(URL, {
		method: "POST",
		headers: {
			"Accept": "application/json",
			"Content-Type": "application/x-www-form-urlencoded"},
		body: params});
	const ret = await response.json();
	console.log(ret);
	return ret;
}

async function display_sim_webpages()
{
	const json_data = await get_site_keywords();

	// similar webpages list
	var swp_lst = document.getElementById("sim-webpages");
	if (swp_lst.textContent.trim() !== '') {
		swp_lst.textContent = '';
	}
	swp_lst.textContent = "Top similar webpages:";

	const swp_json_obj = json_data[1];
	for (const key in swp_json_obj) {
		value = swp_json_obj[key]
		var swp_txt = document.createTextNode(`${value}`);
		var swp_lnk = document.createElement("a");
		var swp_tag = document.createElement("li");
		swp_lnk.setAttribute("href", `https://${value}`);
		swp_lnk.appendChild(swp_txt);
		swp_tag.appendChild(swp_lnk);
		swp_lst.appendChild(swp_tag);
	}

	var par_lst = document.getElementById("par-webpages");	// parameters of webpages
	if (par_lst.textContent.trim() !== '') {
		par_lst.textContent = '';
	}
	par_lst.textContent = "Parameters of similar webpages:";

	const par_json_obj = json_data[2];
	for (const key in par_json_obj) {
		value = par_json_obj[key];
		var par_txt = document.createTextNode(`Registrar: ${value}`);
		var par_tag = document.createElement("li");
		par_tag.appendChild(par_txt);
		par_lst.appendChild(par_tag);
	}
}
