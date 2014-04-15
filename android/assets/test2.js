
{
	"space" :
	{
		properties :
		{
			size : "800*600*200",
			position : "0,0,0",
			shown : "true"
		},
		"menu" :
		{
			properties :
			{
				size : "720*96*0",
				position : "0,0,3",
				links :
				{
					TouchEventMove :
					{
						active : "false",
						factor : "1*1*1"
					}
				},
				interpolators :
				{
					deceleration :
					{
						active : "false",
						factor : "1.5*1.5*1"
					}
				}
			},
			"background" :
			{
				properties :
				{
					image : "black13.png",
					size : "720*96*0",
					position : "0,0,0",
					shown : "false"
				}
			},
			"return" :
			{
				properties :
				{
					image : "hot_return.png",
					size : "96*96*0",
					position : "0,0,0"
				}
			},
			"hot_plus" :
			{
				properties :
				{
					image : "hot_plus.png",
					size : "96*96*0",
					position : "528,0,0"
				}
			},
			"more" :
			{
				properties :
				{
					image : "hot_more.png",
					size : "96*96*0",
					position : "624,0,0"
				}
			}
		},
		"hot" :
		{
			properties :
			{
				size : "720*1280*0",
				position : "0,0,0"
			},
			"main_list" :
			{
				properties :
				{
					size : "720*1280*0",
					position : "0,0,1",
					links :
					{
						TouchEventMove :
						{
							active : "false",
							factor : "1.2*0*1"
						}
					},
					interpolators :
					{
						deceleration :
						{
							active : "false",
							factor : "1.5*0*1"
						}
					}
				},
				"left2_page" :
				{
					properties :
					{
						image : "storm_d.jpg",
						size : "720*1280*0",
						position : "-1440,0,0"
					}
				},
				"left_page" :
				{
					properties :
					{
						image : "rain_n.jpg",
						size : "720*1280*0",
						position : "-720,0,0"
					}
				},
				"this_page" :
				{
					properties :
					{
						image : "clear_d.jpg",
						size : "720*1280*0",
						position : "0,0,0"
					},
					"mini_page_list" :
					{
						properties :
						{
							size : "720*562*0",
							position : "0,720,1",
							links :
							{
								TouchEventMove :
								{
									active : "true",
									factor : "1.2*1.2*1"
								}
							}
						},
						"background" :
						{
							properties :
							{
								image : "black.png",
								size : "720*562*0",
								position : "0,0,0",
								shown : "true"
							}
						},
						list :
						{
							properties :
							{
								size : "720*560*0",
								position : "0,4,10",
								links :
								{
									TouchEventMove :
									{
										active : "false",
										factor : "1.2*0*1"
									}
								},
								interpolators :
								{
									deceleration :
									{
										active : "false",
										factor : "1.5*0*1"
									}
								}
							},
							"page1" :
							{
								properties :
								{
									image : "storm_d.jpg",
									size : "320*560*0",
									position : "0,0,0"
								}
							},
							"page2" :
							{
								properties :
								{
									image : "rain_n.jpg",
									size : "320*560*0",
									position : "324,0,0"
								}
							},
							"page3" :
							{
								properties :
								{
									image : "snow_d.jpg",
									size : "320*560*0",
									position : "648,0,0",
									links :
									{
										TouchEventMove :
										{
											active : "false",
											factor : "0*-1.2*1"
										}
									},
									interpolators :
									{
										deceleration :
										{
											active : "false",
											factor : "0*1.5*0*1"
										}
									}
								}
							},
							"page4" :
							{
								properties :
								{
									image : "snow_n.jpg",
									size : "320*560*0",
									position : "972,0,0"
								}
							},
							"page5" :
							{
								properties :
								{
									image : "rain_d.jpg",
									size : "320*560*0",
									position : "1296,0,70"
								}
							}
						}
					},
					"mini_page_list2" :
					{
						properties :
						{
							size : "720*562*0",
							position : "0,100,1",
							links :
							{
								TouchEventMove :
								{
									active : "true",
									factor : "1.2*1.2*1"
								}
							}
						},
						"background" :
						{
							properties :
							{
								image : "black.png",
								size : "720*562*0",
								position : "0,0,0",
								shown : "true"
							}
						},
						list :
						{
							properties :
							{
								size : "720*560*0",
								position : "0,4,10",
								links :
								{
									TouchEventMove :
									{
										active : "false",
										factor : "1.2*0*1"
									}
								},
								interpolators :
								{
									deceleration :
									{
										active : "false",
										factor : "1.5*0*1"
									}
								}
							},
							"page1" :
							{
								properties :
								{
									image : "storm_d.jpg",
									size : "320*560*0",
									position : "0,0,0"
								}
							},
							"page2" :
							{
								properties :
								{
									image : "rain_n.jpg",
									size : "320*560*0",
									position : "324,0,0"
								}
							},
							"page3" :
							{
								properties :
								{
									image : "snow_d.jpg",
									size : "320*560*0",
									position : "648,0,0",
									links :
									{
										TouchEventMove :
										{
											active : "false",
											factor : "0*-1.2*1"
										}
									},
									interpolators :
									{
										deceleration :
										{
											active : "false",
											factor : "0*1.5*0*1"
										}
									}
								}
							},
							"page4" :
							{
								properties :
								{
									image : "snow_n.jpg",
									size : "320*560*0",
									position : "972,0,0"
								}
							},
							"page5" :
							{
								properties :
								{
									image : "rain_d.jpg",
									size : "320*560*0",
									position : "1296,0,70"
								}
							}
						}
					}
				},
				"right_page" :
				{
					properties :
					{
						image : "snow_n.jpg",
						size : "720*1280*0",
						position : "720,0,0"
					}
				},
				"right2_page" :
				{
					properties :
					{
						image : "rain_d.jpg",
						size : "720*1280*0",
						position : "1440,0,0"
					}
				}
			}
		}
	}
}

{
	"page1" :
	{
		properties :
		{
			image : "storm_d.jpg",
			size : "320*560*0",
			position : "0,0,90"
		}
	},
	"page2" :
	{
		properties :
		{
			image : "rain_n.jpg",
			size : "320*560*0",
			position : "202,0,10"
		}
	}

}
